/*
 * atemsys.c: Provides usermode access to:
 *
 *   - PCI configuration space
 *   - Device IO memory
 *   - Contiguous DMA memory
 *   - Single device interrupt
 *
 * Copyright (c) 2009 - 2016 acontis technologies GmbH, Weingarten, Germany <info@acontis.com>
 * All rights reserved.
 *
 * Author: K. Olbrich <k.olbrich@acontis.com>
 *
 *
 * The driver should be used in the following way:
 *
 * - Make sure this driver's device node is present. I.e. call "mknod /dev/atemsys c 101 0"
 *
 * - open()
 *   Open driver (There can be more then one file descriptor active in parallel).
 *
 * - close()
 *   Close driver. Free resources, if any were allocated.
 *
 * - ioctl(IOCTL_PCI_FIND_DEVICE)
 *   Scan for PCI Devices.
 *   Input:  VendorID, DeviceID, InstanceNo
 *   Output: BusNo, DevNo, FuncNo
 *
 * - ioctl(IOCTL_PCI_CONF_DEVICE)
 *   Configures PCI device. This ioctl pins the given PCI device to the current filedescriptor.
 *   Input:  BusNo, DevNo, FuncNo
 *   Output: Physical IO base address, IO area length, IRQ number
 *   The device must be released explicitly in order to configure the next device. The ioctl gets
 *   errno EBUSY if the device is in use by another device driver.
 *
 * - ioctl(IOCTL_PCI_RELEASE_DEVICE)
 *   Release PCI device and free resources assigned to PCI device (interrupt, DMA memory, ...).
 *
 * - mmap(0, dwSize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, fd, 0);
 *   Allocates and maps DMA memory of size dwSize. Note that the last parameter (offset) must be 0.
 *   Input:  Length in byte
 *   Output: Pointer to the allocated memory and DMA physical address. On success this address is
 *           written into the first 4 bytes of the allocated memory.
 *
 * - mmap(0, IOphysSize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_LOCKED, fd, IOphysAddr);
 *   Maps IO memory of size IOphysSize.
 *   PCI device: 
 *     First call ioctl(IOCTL_PCI_CONF_DEVICE). The IOphysAddr and IOphysSize
 *     parameter must corespond with the base IO address and size returned by
 *     ioctl(IOCTL_PCI_CONF_DEVICE), or the ioctl will fail.
 *   Non-PCI device:
 *     Don't call ioctl(IOCTL_PCI_CONF_DEVICE) before and just pass 
 *     IOphysAddr and IOphysSize. There are no checks done.
 *   Input:  Phys.IO base address, IO area length in byte
 *   Output: Pointer to the mapped IO memory.
 *   The user should call dev_munmap() if the requested DMA memory is not needed anymore. In any cases
 *   the allocated / mapped memory is released / unmapped if the module is unloaded.
 *
 * - ioctl(IOCTL_INT_CONNECT)
 *   Connect an ISR to the device's interrupt.
 *   If the parameter is USE_PCI_INT, then the IRQ is taken from the selected PCI device.
 *   So in this case you have to call ioctl(IOCTL_PCI_CONF_DEVICE) first, or it will fail.
 *   Input:  IRQ-Number or USE_PCI_INT
 *   Output: none
 *   The device interrupt is active if this ioctl succeeds. The caller should do a read() on the file
 *   descriptor. The read call unblocks if an interrupt is received. If the read is unblocked, the
 *   interrupt is disabled on the (A)PIC and the caller must acknowledge the interrupt on the device
 *   (write to mmaped IO register). If the next read() is executed, the interrupt is enabled again
 *   on the (A)PIC. So a missing interrupt acknowledge will held the INT line active and interrupt
 *   trashing will happen (ISR is called again, read() unblocks, ...).
 *   Note that this ioctl will fail with errno EPERM if the interrupt line is shared.
 *   PCI device:
 *     The ioctl will try to use Message Signaled Interrupts (MSI) if supported
 *     by the PCI device. By definition, interrupts are never shared with MSI and MSI are mandatory
 *     for PCI-Express :).
 *
 * - ioctl(IOCTL_INT_DISCONNECT)
 *   Disconnect from device's interrupt.
 *
 * - ioctl(IOCTL_INT_INFO)
 *   Query used interrupt number.
 *
 * - read()
 *   see ioctl(IOCTL_INT_CONNECT)
 *
 *
 *  Changes:
 *
 *  V1.0.00 - Inital, PCI/PCIe only.
 *  V1.1.00 - PowerPC tweaks.
 *           Support for SoC devices (no PCI, i.e. Freescale eTSEC).
 *           Support for current linux kernel's (3.0). Removed deprecated code.
 *  V1.2.00 - 64 bit support. Compat IOCTL's for 32-Bit usermode apps.
 *  V1.2.01 - request_irq() sometimes failed -> Map irq to virq under powerpc.
 *  V1.2.02 - Support for current Linux kernel (3.8.0)
 *  V1.2.03 - Support for current Linux kernel (3.8.13) on armv7l (beaglebone)
 *  V1.2.04 - Use dma_alloc_coherent for arm, because of DMA memory corruption on
 *           Xilinx Zynq.
 *  V1.2.05 - OF Device Tree support for Xilinx Zynq (VIRQ mapping)
 *  V1.2.06 - Wrong major version.
 *  V1.2.07 - Tolerate closing, e.g. due to system()-calls.
 *  V1.2.08 - Add VM_DONTCOPY to prevent crash on system()-calls
 *  V1.2.09 - Apply second controller name change in dts (standard GEM driver for Xilinx Zynq) to avoid default driver loading.
 *  V1.2.10 - Removed IO address alignment to support R6040
 *  V1.2.11 - Fixed lockup in device_read (tLinkOsIst if NIC in interrupt mode) on dev_int_disconnect 
 *  V1.2.12 - Fixed underflow in dev_disable_irq() when more than one interrupts pending because of disable_irq_nosync usage
 *  V1.2.13 - Fixed usage of x64 PCI physical addresses
 *  V1.2.14 - Changes for using with kernel beginnig from 2.6.18
 *  V1.2.15 - Add udev auto-loading support via DTB
 */

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/semaphore.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/irq.h>
#include <linux/list.h>
#include <asm/uaccess.h>
#include <asm/current.h>
#include <linux/compat.h>
#include <linux/slab.h>
#include <linux/device.h>
#ifdef CONFIG_DTC
#  include <linux/of.h>
#  include <linux/of_irq.h>
#endif

#include "atemsys.h"

#if !(defined VM_RESERVED)
# define  VM_RESERVED   (VM_DONTEXPAND | VM_DONTDUMP)
#endif

/* define this if debug messages should be printed to kernel log */
#undef PRINT_DEBUG

/* define this if IO memory should also be mapped into the kernel (for debugging only) */
#undef DEBUG_IOREMAP

#define ATEMSYS_VERSION "1.2.15"

MODULE_AUTHOR("acontis technologies GmbH <info@acontis.com>");
MODULE_DESCRIPTION("Generic usermode PCI driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(ATEMSYS_VERSION);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18))
#   error "At least kernel version 2.6.18 is needed to compile!"
#endif

#if defined(CONFIG_OF)
/* udev auto-loading support via DTB */
static const struct of_device_id atemsys_ids[] = {
    { .compatible = "cdns,gem-ethercat" },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, atemsys_ids);
#endif
 
#define PRINTK(prio, str, ...) printk(prio ATEMSYS_DEVICE_NAME ": " str,  ##__VA_ARGS__)

#define ERR(str, ...) PRINTK(KERN_ERR, str, ##__VA_ARGS__)
#define WRN(str, ...) PRINTK(KERN_WARNING, str, ##__VA_ARGS__)
#define INF(str, ...) PRINTK(KERN_INFO, str, ##__VA_ARGS__)

#ifdef PRINT_DEBUG
#   define DBG(str, ...) PRINTK(KERN_DEBUG, str, ##__VA_ARGS__)
#else
#   define DBG(str, ...) /* do nothing */
#endif

#ifndef PAGE_UP
#   define PAGE_UP(addr)   (((addr)+((PAGE_SIZE)-1))&(~((PAGE_SIZE)-1)))
#endif
#ifndef PAGE_DOWN
#   define PAGE_DOWN(addr) ((addr)&(~((PAGE_SIZE)-1)))
#endif

#ifndef DMA_BIT_MASK
/* Comments: for kernel 2.6.18 add DMA_BIT_MASK*/
#define DMA_BIT_MASK(n) (((n) == 64) ? ~0ULL : ((1ULL<<(n))-1))
#endif
	
typedef struct
{
   u32               irq;
   wait_queue_head_t q;
   atomic_t          count;
   atomic_t          irqStatus;
   atomic_t          totalCount;
} irq_proc;

typedef struct
{
   struct list_head list;
#ifdef CONFIG_PCI   
   struct pci_dev  *pPcidev;
#endif
   irq_proc         irqDesc;
} dev_node;

typedef struct
{
   struct list_head  list;
#ifdef CONFIG_PCI   
   struct pci_dev   *pPcidev;
#endif
   dma_addr_t        dmaAddr;
   void             *pVirtAddr;
   size_t            len;
} mmap_node;


static void dev_munmap(struct vm_area_struct *vma);
static irqreturn_t dev_interrupt_handler(int nIrq, void *pParam);

static DEFINE_MUTEX(S_mtx);
static dev_node S_devNode;
static struct class* S_devClass;
static struct device* S_dev;

static struct vm_operations_struct mmap_vmop =
{
   .close = dev_munmap,
};



static void dev_enable_irq(irq_proc* pIp)
{
   enable_irq(pIp->irq);
   atomic_inc(&pIp->irqStatus);
}

static void dev_disable_irq(irq_proc* pIp)
{
   if (atomic_read(&pIp->irqStatus) > 0)
   {
      disable_irq_nosync(pIp->irq);
      atomic_dec(&pIp->irqStatus);
   }
}

static int dev_irq_disabled(irq_proc* pIp)
{
   if (atomic_read(&pIp->irqStatus) == 0)
   {
      return 1;
   }
   return 0;
}

#if !defined(__arm__)
static void * dev_dma_alloc(u32 dwLen, dma_addr_t *pDmaAddr)
{
   unsigned long virtAddr;
   unsigned long tmpAddr;
   u32 tmpSize;

   virtAddr =  __get_free_pages(GFP_KERNEL | GFP_DMA, get_order(dwLen));
   if (! virtAddr)
   {
      ERR("mmap: __get_free_pages failed\n");
      return NULL;
   }

   tmpAddr = virtAddr;
   tmpSize = dwLen;

   while (tmpSize > 0)
   {
     SetPageReserved( virt_to_page(tmpAddr) );
     tmpAddr += PAGE_SIZE;
     tmpSize -= PAGE_SIZE;
   }

   *pDmaAddr = virt_to_phys((void *) virtAddr);

   return (void *) virtAddr;
}

static void dev_dma_free(u32 dwLen, void *virtAddr)
{
   unsigned long tmpAddr = (unsigned long) virtAddr;
   u32 tmpSize = dwLen;

   while (tmpSize > 0)
   {
     ClearPageReserved( virt_to_page(tmpAddr) );
     tmpAddr += PAGE_SIZE;
     tmpSize -= PAGE_SIZE;
   }

   free_pages((unsigned long) virtAddr, get_order(dwLen));
}
#endif /* !__arm__ */

static void dev_munmap(struct vm_area_struct *vma)
{
   mmap_node *pMnode = (mmap_node *) vma->vm_private_data;

   INF("dev_munmap: 0x%p -> 0x%p (%d)\n",
         (void *) pMnode->pVirtAddr, (void *)(unsigned long)pMnode->dmaAddr, (int) pMnode->len);
	if (0 == pMnode->dmaAddr) { INF("dev_munmap: 0 == pMnode->dmaAddr!\n"); return; }
	if (NULL == pMnode->pVirtAddr) { INF("dev_munmap: NULL == pMnode->pVirtAddr!\n"); return; }

   /* free DMA memory */
#ifdef CONFIG_PCI   
   if (pMnode->pPcidev == NULL)
#endif
   {
#  ifdef __arm__
      dma_free_coherent(S_dev, pMnode->len, pMnode->pVirtAddr, pMnode->dmaAddr);
#  else
      dev_dma_free(pMnode->len, pMnode->pVirtAddr);
#  endif
   }
#ifdef CONFIG_PCI   
   else
   {
      pci_free_consistent(pMnode->pPcidev, pMnode->len, pMnode->pVirtAddr, pMnode->dmaAddr);
   }
#endif
   kfree(pMnode);
}

#ifdef CONFIG_PCI
/*
 * Lookup PCI device
 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
struct pci_dev *pci_get_bus_and_slot(unsigned int bus, unsigned int devfn)
{
	struct pci_dev *dev = NULL;

	for_each_pci_dev(dev) {
		if (pci_domain_nr(dev->bus) == 0 &&
		    (dev->bus->number == bus && dev->devfn == devfn))
			return dev;
	}
	return dev;
}
#endif

static int dev_pci_select_device(dev_node* pDevDesc, PCI_SELECT_DESC* pciDesc)
{
   int nRetval = -EFAULT;
   s32 nPciBus, nPciDev, nPciFun;

   get_user(nPciBus, &pciDesc->nPciBus);
   get_user(nPciDev, &pciDesc->nPciDev);
   get_user(nPciFun, &pciDesc->nPciFun);

   INF("pci_select: %02x:%02x.%x\n",
         (u32) nPciBus, (u32) nPciDev, (u32) nPciFun);

   /* Lookup pci_dev object */
   pDevDesc->pPcidev = pci_get_bus_and_slot(nPciBus, PCI_DEVFN(nPciDev, nPciFun));
   if (pDevDesc->pPcidev == NULL)
   {
      WRN("pci_select: PCI-Device %02x:%02x.%x not found\n",
            (unsigned) nPciBus, (unsigned) nPciDev, (unsigned) nPciFun);
      goto Exit;
   }

   nRetval = DRIVER_SUCCESS;

Exit:
   return nRetval;
}

/*
 * See also kernel/Documentation/PCI/pci.txt for the recommended PCI initialization sequence
 */
static int ioctl_pci_configure_device(dev_node* pDevDesc, unsigned long ioctlParam)
{
   int nRetval = -EIO;
   int nRc;
   int i;
   unsigned long ioBase;
   u32 dwIOLen;
   s32 nBar = 0;
   PCI_SELECT_DESC *pPciDesc = (PCI_SELECT_DESC *) ioctlParam;

   if (! access_ok(VERIFY_WRITE, pPciDesc, sizeof(PCI_SELECT_DESC)))
   {
      ERR("pci_conf: EFAULT\n");
      nRetval = -EFAULT;
      goto Exit;
   }

   if (pDevDesc->pPcidev != NULL)
   {
      WRN("pci_conf: error call ioctl(IOCTL_PCI_RELEASE_DEVICE) first\n");
      goto Exit;
   }

   if (dev_pci_select_device(pDevDesc, pPciDesc) != DRIVER_SUCCESS)
   {
      goto Exit;
   }

   /* enable device */
   nRc = pci_enable_device(pDevDesc->pPcidev);
   if (nRc < 0)
   {
      ERR("pci_conf: pci_enable_device failed\n");
      pDevDesc->pPcidev = NULL;
      goto Exit;
   }

   /* Check if IO-memory is in use by another driver */
   nRc = pci_request_regions(pDevDesc->pPcidev, ATEMSYS_DEVICE_NAME);
   if (nRc < 0)
   {
      ERR("pci_conf: device \"%s\" in use by another driver?\n", pci_name(pDevDesc->pPcidev));
      pDevDesc->pPcidev = NULL;
      nRetval = -EBUSY;
      goto Exit;
   }

   /* find the memory BAR */
   for (i = 0; i < PCI_MAXBAR ; i++)
   {
      if (pci_resource_flags(pDevDesc->pPcidev, i) & IORESOURCE_MEM)
      {
         /* IO area address */
         ioBase = pci_resource_start(pDevDesc->pPcidev, i);

         if (ioBase > 0xFFFFFFFF)
         {
            ERR("pci_conf: 64-Bit IO address not supported\n");
            pDevDesc->pPcidev = NULL;
            nRetval = -ENODEV;
            goto Exit;
         }

         put_user((u32)ioBase, &(pPciDesc->aBar[nBar].dwIOMem));

         /* IO area length */
         dwIOLen = pci_resource_len(pDevDesc->pPcidev, i);
         put_user(dwIOLen, &(pPciDesc->aBar[nBar].dwIOLen));

         nBar++;
      }
   }

   if (nBar == 0)
   {
      ERR("pci_conf: No memory BAR found\n");
      pDevDesc->pPcidev = NULL;
      nRetval = -ENODEV;
      goto Exit;
   }

   put_user(nBar, &(pPciDesc->nBarCnt)); /* number of memory BARs */

   /* Turn on Memory-Write-Invalidate if it is supported by the device*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
   pci_set_mwi(pDevDesc->pPcidev);
#else
   pci_try_set_mwi(pDevDesc->pPcidev);
#endif

   /* Set DMA mask. We can handle only 32 bit DMA addresses! */
   nRc = pci_set_dma_mask(pDevDesc->pPcidev, DMA_BIT_MASK(32));
   if (nRc < 0)
   {
      ERR("pci_conf: pci_set_dma_mask failed\n");
      pci_release_regions(pDevDesc->pPcidev);
      pDevDesc->pPcidev = NULL;
      goto Exit;
   }
   nRc = pci_set_consistent_dma_mask(pDevDesc->pPcidev, DMA_BIT_MASK(32));
   if (nRc < 0)
   {
      ERR("pci_conf: pci_set_consistent_dma_mask failed\n");
      pci_release_regions(pDevDesc->pPcidev);
      pDevDesc->pPcidev = NULL;
      goto Exit;
   }

   /* Enable bus master DMA */
   pci_set_master(pDevDesc->pPcidev);

   /* Try to enable MSI (Message Signaled Interrupts). MSI's are non shared, so we can
    * use interrupt mode, also if we have a non exclusive interrupt line with legacy
    * interrupts.
    */
   if (pci_enable_msi(pDevDesc->pPcidev))
   {
      INF("pci_conf: legacy INT configured for device %s\n", pci_name(pDevDesc->pPcidev));
   }
   else
   {
      INF("pci_conf: MSI configured for device %s\n", pci_name(pDevDesc->pPcidev));
   }

   put_user((u32)pDevDesc->pPcidev->irq, &(pPciDesc->dwIrq)); /* assigned IRQ */

#if defined(__arm__) && 0
   /*
    * This is required for TI's TMDXEVM8168 (Cortex A8) eval board
    * \sa TI "DM81xx AM38xx PCI Express Root Complex Driver User Guide"
    * "DM81xx RC supports maximum remote read request size (MRRQS) as 256 bytes"
    */
   pcie_set_readrq(pDevDesc->pPcidev, 256);
#endif

   nRetval = 0;

Exit:
   return nRetval;
}

static int ioctl_pci_finddevice(dev_node* pDevDesc, unsigned long ioctlParam)
{
   int nRetval = -EIO;
   struct pci_dev* pPciDev = NULL;
   s32 nVendor, nDevice, nInstance, nInstanceId;
   PCI_SELECT_DESC* pPciDesc = (PCI_SELECT_DESC *) ioctlParam;

   if (! access_ok(VERIFY_WRITE, pPciDesc, sizeof(PCI_SELECT_DESC)))
   {
      ERR("pci_find: EFAULT\n");
      nRetval = -EFAULT;
      goto Exit;
   }

   get_user(nVendor, &pPciDesc->nVendID);
   get_user(nDevice, &pPciDesc->nDevID);
   get_user(nInstance, &pPciDesc->nInstance);

   INF("pci_find: ven 0x%x dev 0x%x nInstance %d\n", nVendor, nDevice, nInstance);

   for (nInstanceId = 0; nInstanceId <= nInstance; nInstanceId++ )
   {
      pPciDev = pci_get_device (nVendor, nDevice, pPciDev);
   }

   if (pPciDev == NULL)
   {
      WRN("pci_find: device 0x%x:0x%x:%d not found\n", nVendor, nDevice, nInstance);
      nRetval = -ENODEV;
      goto Exit;
   }

   INF("pci_find: found 0x%x:0x%x:%d -> %s\n",
       nVendor, nDevice, nInstance, pci_name(pPciDev));

   put_user((s32)pPciDev->bus->number, &pPciDesc->nPciBus); /* Bus */
   put_user((s32)PCI_SLOT(pPciDev->devfn), &pPciDesc->nPciDev); /* Device */
   put_user((s32)PCI_FUNC(pPciDev->devfn), &pPciDesc->nPciFun); /* Function */

   nRetval = 0;

Exit:
   return nRetval;
}
#endif /* ifdef CONFIG_PCI */

#ifdef CONFIG_DTC

/* 
 * Lookup Nth (0: first) compatible device tree node with "interrupts" property present. 
 */
static struct device_node * atemsys_of_lookup_intnode(const char *compatible, int deviceIdx)
{
   struct device_node *device = NULL;
   struct device_node *child = NULL;
   struct device_node *tmp = NULL;
   int devCnt;

   /* Lookup Nth device tree node */
   devCnt = 0;
   for_each_compatible_node(tmp, NULL, compatible)
   {
      if (devCnt == deviceIdx)
      {
         device = tmp;
         break;
      }
      ++devCnt;
   }
   
   if (device == NULL) return NULL;

   if (of_get_property(device, "interrupts", NULL)) return device;
   
   /* i.e. vETSEC has 2 groups. Search them */
   for_each_child_of_node(device, child)
   {
      if (of_get_property(child, "interrupts", NULL)) return child;
   }
   
   return NULL;
}

/*
 * Map interrupt number taken from the OF Device Tree (\sa .dts file) into
 * virtual interrupt number which can be passed to request_irq().
 * The usual (device driver) way is to use the irq_of_parse_and_map() function. 
 *
 * We search all device tree nodes which have the "compatible" property
 * equal to compatible. Search until the Nth device is found. Then
 * map the Nth interrupt (given by intIdx) with irq_of_parse_and_map().
 */
static unsigned atemsys_of_map_irq_to_virq(const char *compatible, int deviceIdx, int intIdx)
{
   unsigned virq;
   struct device_node *device = NULL;

   /* Lookup Nth device */
   device = atemsys_of_lookup_intnode(compatible, deviceIdx);
   if (! device)
   {
      ERR("atemsys_of_map_irq_to_virq: device tree node '%s':%d not found.\n",
         compatible, deviceIdx);
      return NO_IRQ;
   }
   
   virq = irq_of_parse_and_map(device, intIdx);
   if (virq == NO_IRQ)
   {
      ERR("atemsys_of_map_irq_to_virq: irq_of_parse_and_map failed for"
          " device tree node '%s':%d, IntIdx %d.\n",
         compatible, deviceIdx, intIdx);
   }
   
   return virq;
}

#endif /* ifdef CONFIG_DTC */


static int ioctl_int_connect(dev_node* pDevDesc, unsigned long ioctlParam)
{
   int nRetval = -EIO;
   int nRc;
   irq_proc *pIp = NULL;
   unsigned int irq = 0;

#ifdef CONFIG_PCI   
   if (ioctlParam == USE_PCI_INT)
   {
      /* Use IRQ number from selected PCI device */

      if (pDevDesc->pPcidev == NULL)
      {
         WRN("intcon: error call ioctl(IOCTL_PCI_SELECT_DEVICE) first\n");
         goto Exit;
      }

      irq = pDevDesc->pPcidev->irq;
      INF("intcon: Use IRQ (%d) from PCI config\n", irq);
   }
   else
#endif /* ifdef CONFIG_PCI */
   {
#ifdef CONFIG_DTC
      /* The ioctlParam is the Nth compatible device in the OF device tree (0: first, 1: second, ...)
       * TODO "compatible string" and "interrupt index" should be provided by usermode as IOCTL param
       */
      if ( /* Use interrupt number at idx 1 (Rx-Interrupt) for TSEC / eTSEC */
             ((irq = atemsys_of_map_irq_to_virq("fsl,etsec2", ioctlParam, 1)) == NO_IRQ) /* PPC, eTSEC */
          && ((irq = atemsys_of_map_irq_to_virq("gianfar", ioctlParam, 1)) == NO_IRQ) /* PPC, eTSEC */
          /* standard GEM driver for Xilinx Zynq, second controller name changed in dts to avoid default driver loading */
          && ((irq = atemsys_of_map_irq_to_virq("cdns,gem-ethercat", ioctlParam, 0)) == NO_IRQ)
          /* Use interrupt number at idx 0 (Catch-All-Interrupt) for GEM */
          && ((irq = atemsys_of_map_irq_to_virq("xlnx,ps7-ethernet-1.00.a", ioctlParam, 0)) == NO_IRQ) /* ARM, Xilinx Zynq */
         )
      {
         nRetval = -EPERM;
         goto Exit;
      }
      INF("intcon: Use IRQ (%d) from OF Device Tree\n", irq);
#else
      /* Use IRQ number passed as ioctl argument */
      irq = ioctlParam;
      INF("intcon: Use IRQ (%d) passed by user\n", irq);
#endif
   }

   pIp = &pDevDesc->irqDesc;
   if (pIp->irq)
   {
      WRN("intcon: error IRQ %u already connected. Call ioctl(IOCTL_INT_DISCONNECT) first\n",
            (unsigned) pIp->irq);
      goto Exit;
   }

   /* Setup some data which is needed during Interrupt handling */
   memset(pIp, 0, sizeof(irq_proc));
   init_waitqueue_head(&pIp->q);
   atomic_set(&pIp->count, 0);
   atomic_set(&pIp->totalCount, 0);
   atomic_set(&pIp->irqStatus, 1); /* IRQ enabled */

   /* Setup non shared IRQ */
   nRc = request_irq(irq, dev_interrupt_handler, 0, ATEMSYS_DEVICE_NAME, pDevDesc);
   if (nRc)
   {
      ERR("intcon: request_irq (IRQ %d) failed. Err %d\n", irq, nRc);
      nRetval = -EPERM;
      goto Exit;
   }

   pIp->irq = irq;

   INF("intcon: IRQ %d connected\n", irq);

   nRetval = 0;

Exit:
   return nRetval;
}

static int ioctl_intinfo(dev_node* pDevDesc, unsigned long ioctlParam)
{
   int nRetval = -EIO;
   INT_INFO *pIntInfo = (INT_INFO *) ioctlParam;

   if (! access_ok(VERIFY_WRITE, pIntInfo, sizeof(INT_INFO)))
   {
      ERR("ioctl_intinfo: EFAULT\n");
      nRetval = -EFAULT;
      goto Exit;
   }
   
   put_user(pDevDesc->irqDesc.irq, &pIntInfo->dwInterrupt);

   nRetval = 0;

Exit:
   return nRetval;
}


static void dev_int_disconnect(dev_node* pDevDesc)
{
   int nCnt;
   irq_proc *pIp = &(pDevDesc->irqDesc);

   if (pIp->irq)
   {
      /* Disable INT line. We can call this, because we only allow exclusive interrupts */
      disable_irq_nosync(pIp->irq);

      /* Unregister INT routine.This will block until all pending interrupts are handled */
      free_irq(pIp->irq, pDevDesc);

      nCnt = atomic_read(&pIp->totalCount);
      INF("pci_intdcon: IRQ %u disconnected. %d interrupts rcvd\n", (u32) pIp->irq, nCnt);

      pIp->irq = 0;

      /* Wakeup sleeping threads -> read() */
      wake_up(&pIp->q);
   }
}

#ifdef CONFIG_PCI
static void dev_pci_release(dev_node* pDevDesc)
{
   if (pDevDesc->pPcidev)
   {
      pci_disable_device(pDevDesc->pPcidev);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,29))
      /* Make sure bus master DMA is disabled if the DMA buffers are finally released */
      pci_clear_master(pDevDesc->pPcidev);
#endif
      pci_release_regions(pDevDesc->pPcidev);

      pci_disable_msi(pDevDesc->pPcidev);

      INF("pci_release: PCI device %s released\n", pci_name(pDevDesc->pPcidev));

      pDevDesc->pPcidev = NULL;
   }
}
#endif /* ifdef CONFIG_PCI */

static irqreturn_t dev_interrupt_handler(int nIrq, void *pParam)
{
   dev_node* pDevDesc = (dev_node *) pParam;
   irq_proc* pIp = &(pDevDesc->irqDesc);

   /* Disable IRQ on (A)PIC to prevent interrupt trashing if the ISR is left.
    * In usermode the IRQ must be acknowledged on the device (IO register).
    * The IRQ is enabled again in the read() handler!
    * Just disabling the IRQ here doesn't work with shared IRQs!
    */
   dev_disable_irq(pIp);

   atomic_inc(&pIp->count);
   atomic_inc(&pIp->totalCount);

   /* Wakeup sleeping threads -> read() */
   wake_up(&pIp->q);

   return IRQ_HANDLED;
}

/*
 * This is called whenever a process attempts to open the device file
 */
static int device_open(struct inode *inode, struct file *file)
{
   dev_node* pDevDesc;

   INF("device_open(0x%p)\n", file);

   /* create device descriptor */
   pDevDesc = (dev_node *) kzalloc(sizeof(dev_node), GFP_KERNEL);
   if (pDevDesc == NULL)
   {
      return -ENOMEM;
   }

   file->private_data = (void *) pDevDesc;

   /* Add descriptor to descriptor list */
   mutex_lock(&S_mtx);
   list_add(&pDevDesc->list, &S_devNode.list);
   mutex_unlock(&S_mtx);

   try_module_get(THIS_MODULE);

   return DRIVER_SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
   dev_node* pDevDesc = file->private_data;

   /* release device descriptor */
   if (pDevDesc != NULL )
   {
       INF("device_release, pDevDesc = 0x%p\n", pDevDesc);

       /* Try to tear down interrupts if they are on */
       dev_int_disconnect(pDevDesc);
#ifdef CONFIG_PCI
       /* Try to release PCI resources */
       dev_pci_release(pDevDesc);
#endif
       /* Remove descriptor from descriptor list */
       mutex_lock(&S_mtx);

       list_del(&pDevDesc->list);

       mutex_unlock(&S_mtx);

       kfree(pDevDesc);
   }

   module_put(THIS_MODULE);

   return DRIVER_SUCCESS;
}

/*
 * This function is called whenever a process which has already opened the
 * device file attempts to read from it.
 */
static ssize_t device_read(
      struct file *filp,   /* see include/linux/fs.h   */
      char __user *bufp,   /* buffer to be filled with data */
      size_t       len,    /* length of the buffer     */
      loff_t      *ppos)
{

   dev_node* pDevDesc = (dev_node *) filp->private_data;
   irq_proc* pIp = NULL;
   s32 nPending;
   wait_queue_t wait;

   if (! pDevDesc)
   {
      return -EINVAL;
   }

   pIp = &(pDevDesc->irqDesc);

   /* DBG("device_read...(0x%p,0x%p,%d)\n", filp, bufp, len); */

   init_wait(&wait);

   if (len < sizeof(u32))
   {
      return -EINVAL;
   }

   if (pIp->irq == 0) /* IRQ already disabled */
   {
      return -EINVAL;
   }

   nPending = atomic_read(&pIp->count);
   if (nPending == 0)
   {
      if (dev_irq_disabled(pIp))
      {
         dev_enable_irq(pIp);
      }
      if (filp->f_flags & O_NONBLOCK)
      {
         return -EWOULDBLOCK;
      }
   }

   while (nPending == 0)
   {
      prepare_to_wait(&pIp->q, &wait, TASK_INTERRUPTIBLE);
      nPending = atomic_read(&pIp->count);
      if (nPending == 0)
      {
         schedule();
      }
      finish_wait(&pIp->q, &wait);
      if (pIp->irq == 0) /* IRQ disabled while waiting for IRQ */
      {
         return -EINVAL;
      }
      if (signal_pending(current))
      {
         return -ERESTARTSYS;
      }
   }

   if (copy_to_user(bufp, &nPending, sizeof(nPending)))
   {
      return -EFAULT;
   }

   *ppos += sizeof(nPending);
   atomic_sub(nPending, &pIp->count);

   return sizeof(nPending);
}

/*
 * character device mmap method
 */
static int device_mmap(struct file *filp, struct vm_area_struct *vma)
{
   dev_node*   pDevDesc = filp->private_data;
   int         nRet = -EIO;
   u32         dwLen;
   void       *pVa = NULL;
   dma_addr_t  dmaAddr;
   mmap_node  *pMmapNode;
#ifdef CONFIG_PCI
   int         i;
   unsigned long ioBase;
   u32 dwIOLen, dwPageOffset;
#endif

   DBG("mmap: vm_pgoff 0x%p vm_start = 0x%p vm_end = 0x%p\n",
         (void *) vma->vm_pgoff, (void *) vma->vm_start, (void *) vma->vm_end);

   if (pDevDesc == NULL)
   {
      ERR("mmap: Invalid device dtor\n");
      goto Exit;
   }

   dwLen = PAGE_UP(vma->vm_end - vma->vm_start);

   vma->vm_flags |= VM_RESERVED | VM_LOCKED | VM_DONTCOPY;

   if (vma->vm_pgoff != 0)
   {
      /* map device IO memory */
#ifdef CONFIG_PCI
      if (pDevDesc->pPcidev != NULL)
      {
         INF("mmap: Doing PCI device sanity check\n");

         /* sanity check. Make sure that the offset parameter of the mmap() call in userspace
          * corresponds with the PCI base IO address.
          * Make sure the user doesn't map more IO memory than the device provides.
          */
         for (i = 0; i < PCI_MAXBAR; i++)
         {
            if (pci_resource_flags(pDevDesc->pPcidev, i) & IORESOURCE_MEM)
            {
               /* IO area address */
               ioBase = PAGE_DOWN( pci_resource_start(pDevDesc->pPcidev, i) );

               dwPageOffset = pci_resource_start(pDevDesc->pPcidev, i) - ioBase;

               /* IO area length */
               dwIOLen = PAGE_UP( pci_resource_len(pDevDesc->pPcidev, i) + dwPageOffset );

               if (    ((vma->vm_pgoff << PAGE_SHIFT) >= ioBase)
                    && (((vma->vm_pgoff << PAGE_SHIFT) + dwLen) <= (ioBase + dwIOLen))
                  )
               {
                  /* for systems where physical address is in x64 space, high dword is not passes from user io
                   * use correct address from pci_resource_start */
                  resource_size_t res_start = pci_resource_start(pDevDesc->pPcidev, i);
            	  unsigned long pgoff_new = (res_start>>PAGE_SHIFT);
            	  if (pgoff_new != vma->vm_pgoff)
            	  {
            		  INF("mmap: Correcting page offset from 0x%lx to 0x%lx, for Phys address 0x%llx",
            				  vma->vm_pgoff, pgoff_new, (u64)res_start);
            		  vma->vm_pgoff =  pgoff_new;
            	  }

                  break;
               }
            }
         }

         /* IO bar not found? */
         if (i == PCI_MAXBAR)
         {
            ERR("mmap: Invalid arguments\n");
            nRet = -EINVAL;
            goto Exit;
         }
      }
#endif /* ifdef CONFIG_PCI */

      /* avoid swapping, request IO memory */
      vma->vm_flags |= VM_IO;

      /*
       * avoid caching (this is at least needed for POWERPC,
       * or machine will lock on first IO access)
       */
      vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

      if ((nRet = remap_pfn_range(vma,
                                 vma->vm_start,
                                 vma->vm_pgoff,
                                 dwLen,
                                 vma->vm_page_prot)) < 0)
      {
         ERR("mmap: remap_pfn_range failed\n");
         goto Exit;
      }

      INF("mmap: mapped IO memory, Phys:0x%llx UVirt:0x%p Size:%u\n",
           (u64) (((u64)vma->vm_pgoff) << PAGE_SHIFT), (void *) vma->vm_start, dwLen);
           
#ifdef DEBUG_IOREMAP
      {
        volatile unsigned char *ioaddr;
        unsigned long ioBase = vma->vm_pgoff << PAGE_SHIFT;
        printk("try to remap %p\n", (void *)ioBase); 
        /* DEBUG Map device's IO memory into kernel space pagetables */
        ioaddr = (volatile unsigned char *) ioremap_nocache(ioBase, dwLen);
        if (ioaddr == NULL)
        {
          ERR("ioremap_nocache failed\n");
          goto Exit;
        }
        printk("io_base %p, *io_base[0]: %08x\n", ioaddr, readl(ioaddr));
      }
#endif /* ifdef DEBUG_IOREMAP */
   }
   else
   {
      /* allocated and map DMA memory */
#ifdef CONFIG_PCI
      if (pDevDesc->pPcidev != NULL)
      {
         pVa = pci_alloc_consistent(pDevDesc->pPcidev, dwLen, &dmaAddr);
         if (! pVa)
         {
            ERR("mmap: pci_alloc_consistent failed\n");
            nRet = -ENOMEM;
            goto Exit;
         }
      }
      else
#endif /* CONFIG_PCI */
      {
#ifdef __arm__
         /* dma_alloc_coherent() is currently not tested on PPC.
          * TODO test this and remove legacy dev_dma_alloc()
          */
         pVa = dma_alloc_coherent(S_dev, dwLen, &dmaAddr, GFP_KERNEL);
#else
         pVa = dev_dma_alloc(dwLen, &dmaAddr);
#endif
         if (! pVa)
         {
           nRet = -ENOMEM;
           goto Exit;
         }
      }

      if (dmaAddr > 0xFFFFFFFF)
      {
         ERR("mmap: Can't handle 64-Bit DMA address\n");
         nRet = -ENOMEM;
         goto ExitAndFree;
      }

      /* zero memory for security reasons */
      memset(pVa, 0, dwLen);

      /* Always use noncached DMA memory for ARM. Otherwise cache invaliation/sync
       * would be necessary from usermode. 
       * Can't do that without a kernel call because this OP's are privileged.
       */
#ifdef __arm__ 
      vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
#endif

      /* map the whole physically contiguous area in one piece */
      if ((nRet = remap_pfn_range(vma,
                                 vma->vm_start,
                                 dmaAddr >> PAGE_SHIFT,
                                 dwLen,
                                 vma->vm_page_prot)) < 0)
      {
         ERR("remap_pfn_range failed\n");
         goto ExitAndFree;
      }

      /* Write the physical DMA address into the first 4 bytes of allocated memory */
      *((u32 *) pVa) = (u32) dmaAddr;

      /* Some housekeeping to be able to cleanup the allocated memory later */
      pMmapNode = kzalloc(sizeof(mmap_node), GFP_KERNEL);
      if (! pMmapNode)
      {
         ERR("mmap: kmalloc() failed\n");
         nRet = -ENOMEM;
         goto ExitAndFree;
      }
      
#ifdef CONFIG_PCI
      pMmapNode->pPcidev = pDevDesc->pPcidev;
#endif
      pMmapNode->dmaAddr = dmaAddr;
      pMmapNode->pVirtAddr = pVa;
      pMmapNode->len = dwLen;

      /* Setup close callback -> deallocates DMA memory if region is unmapped by the system */
      vma->vm_ops = &mmap_vmop;
      vma->vm_private_data = pMmapNode;

      INF("mmap: mapped DMA memory, Phys:0x%p KVirt:0x%p UVirt:0x%p Size:%u\n",
             (void *)(unsigned long)dmaAddr, (void *)pVa, (void *)vma->vm_start, dwLen);
   }

   nRet = 0;

   goto Exit;

ExitAndFree:

   if (pVa == NULL) goto Exit;

#ifdef CONFIG_PCI   
   if (pDevDesc->pPcidev == NULL)
#endif
   {
#ifdef __arm__
      dma_free_coherent(S_dev, dwLen, pVa, dmaAddr);
#else
      dev_dma_free(dwLen, pVa);
#endif
   }
#ifdef CONFIG_PCI   
   else
   {
      pci_free_consistent(pDevDesc->pPcidev, dwLen, pVa, dmaAddr);
   }
#endif
Exit:
   return nRet;
}

/*
 * This function is called whenever a process tries to do an ioctl on our
 * device file.
 *
 * If the ioctl is write or read/write (meaning output is returned to the
 * calling process), the ioctl call returns the output of this function.
 *
 */
static long atemsys_ioctl(
      struct file *file,
      unsigned int cmd,
      unsigned long arg)
{
   dev_node*        pDevDesc = file->private_data;
   int              nRetval = -EFAULT;

   if (pDevDesc == NULL)
   {
      ERR("ioctl: Invalid device dtor\n");
      goto Exit;
   }

   /*
    * Switch according to the ioctl called
    */
   switch (cmd)
   {
#ifdef CONFIG_PCI   
      case IOCTL_PCI_FIND_DEVICE:
      {
         if (ioctl_pci_finddevice(pDevDesc, arg) < 0)
         {
           goto Exit;
         }
         break;
      }

      case IOCTL_PCI_CONF_DEVICE:
      {
         if (ioctl_pci_configure_device(pDevDesc, arg) < 0)
         {
            goto Exit;
         }
         break;
      }

      case IOCTL_PCI_RELEASE_DEVICE:
      {
         if (pDevDesc->pPcidev == NULL)
         {
            DBG("pci_release: No PCI device selected. Call ioctl(IOCTL_PCI_SELECT_DEVICE) first\n");
            goto Exit;
         }

         dev_pci_release(pDevDesc);
         break;
      }
#endif
      case IOCTL_INT_CONNECT:
      {
         if (ioctl_int_connect(pDevDesc, arg) < 0)
         {
            goto Exit;
         }
         break;
      }

      case IOCTL_INT_DISCONNECT:
      {
         dev_int_disconnect(pDevDesc);
         break;
      }

      case IOCTL_INT_INFO:
      { 
         if (ioctl_intinfo(pDevDesc, arg) < 0)
         {
            goto Exit;
         }
         break;
      }

      default:
      {
         nRetval = -ENODEV;
         goto Exit;
         break;
      }
   }

   nRetval = DRIVER_SUCCESS;

Exit:
   return nRetval;
}

#ifdef CONFIG_COMPAT
/*
 * ioctl processing for 32 bit process on 64 bit system
 */
static long atemsys_compat_ioctl(
      struct file *file,
      unsigned int cmd,
      unsigned long arg)
{
   return atemsys_ioctl(file, cmd, (unsigned long) compat_ptr(arg));
}
#endif

/* Module Declarations */

/*
 * This structure will hold the functions to be called
 * when a process does something to the device we
 * created. Since a pointer to this structure is kept in
 * the devices table, it can't be local to
 * module_init. NULL is for unimplemented functions.
 */
struct file_operations Fops = {
   .read = device_read,
   .unlocked_ioctl = atemsys_ioctl,
#ifdef CONFIG_COMPAT
   .compat_ioctl = atemsys_compat_ioctl, /* ioctl processing for 32 bit process on 64 bit system */
#endif
   .open = device_open,
   .mmap = device_mmap,
   .release = device_release,   /* a.k.a. close */
};

/*
 * Initialize the module - Register the character device
 */
int init_module()
{
   /* Register the character device */
   int major = register_chrdev(MAJOR_NUM, ATEMSYS_DEVICE_NAME, &Fops);
   if (major < 0)
   {
      INF("Failed to register %s (err: %d)\n",
             ATEMSYS_DEVICE_NAME, major);
      return major;
   }

   S_devClass = class_create(THIS_MODULE, ATEMSYS_DEVICE_NAME);
	if (IS_ERR(S_devClass))
   {
      INF("class_create failed\n");
      return -1;
   }
   
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
   S_dev = class_device_create(S_devClass, NULL, MKDEV(MAJOR_NUM, 0), NULL, ATEMSYS_DEVICE_NAME);
#else
   S_dev = device_create(S_devClass, NULL, MKDEV(MAJOR_NUM, 0), NULL, ATEMSYS_DEVICE_NAME);
#endif

   if (IS_ERR(S_dev))
   {
      INF("device_create failed\n");
      return -1;
   }
   S_dev->coherent_dma_mask = 0xFFFFFFFF;
   
   INIT_LIST_HEAD(&S_devNode.list);

   INF("%s v%s loaded\n", ATEMSYS_DEVICE_NAME, ATEMSYS_VERSION);

   return 0;
}

/*
 * Cleanup - unregister the appropriate file from /proc
 */
void cleanup_module()
{
   INF("%s unloaded\n", ATEMSYS_DEVICE_NAME);

   device_destroy(S_devClass, MKDEV(MAJOR_NUM, 0));
   class_destroy(S_devClass);
   unregister_chrdev(MAJOR_NUM, ATEMSYS_DEVICE_NAME);
}
