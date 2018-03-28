/*
 * atemsys.h: atemsys.ko headerfile
 *
 * Copyright (c) 2009 - 2012 acontis technologies GmbH, Weingarten, Germany <info@acontis.com>
 * All rights reserved.
 *
 * Note: This header is also included by userspace!
 */

#ifndef ATEMSYS_H
#define ATEMSYS_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define DRIVER_SUCCESS  0

/*
 * The major device number. We can't rely on dynamic
 * registration any more, because ioctls need to know
 * it.
 */
#define MAJOR_NUM 101

#define IOCTL_PCI_FIND_DEVICE    _IOWR(MAJOR_NUM, 0, PCI_SELECT_DESC)
#define IOCTL_PCI_CONF_DEVICE    _IOWR(MAJOR_NUM, 1, PCI_SELECT_DESC)
#define IOCTL_PCI_RELEASE_DEVICE _IO(MAJOR_NUM, 2)
#define IOCTL_INT_CONNECT        _IOW(MAJOR_NUM, 3, __u32)
#define IOCTL_INT_DISCONNECT     _IOW(MAJOR_NUM, 4, __u32)
#define IOCTL_INT_INFO           _IOR(MAJOR_NUM, 5, INT_INFO)

/*
 * The name of the device driver
 */
#define ATEMSYS_DEVICE_NAME "atemsys"

/*
 * The name of the device file
 */
#define ATEMSYS_FILE_NAME "/dev/" ATEMSYS_DEVICE_NAME

#define PCI_MAXBAR (6)
#define USE_PCI_INT (0xFFFFFFFF) /* Query the selected PCI device for the assigned IRQ number */

typedef struct
{
   __u32  dwIOMem;           /* [out] IO Memory of PCI card (physical address) */
   __u32  dwIOLen;           /* [out] Length of the IO Memory area*/
} __attribute__((packed)) PCI_MEMBAR;

typedef struct
{
   __s32        nVendID;          /* [in] vendor ID */
   __s32        nDevID;           /* [in] device ID */
   __s32        nInstance;        /* [in] instance to look for (0 is the first instance) */
   __s32        nPciBus;          /* [in/out] bus */
   __s32        nPciDev;          /* [in/out] device */
   __s32        nPciFun;          /* [in/out] function */
   __s32        nBarCnt;          /* [out] Number of entries in aBar */
   __u32        dwIrq;            /* [out] IRQ or USE_PCI_INT */
   PCI_MEMBAR   aBar[PCI_MAXBAR]; /* [out] IO memory */
} __attribute__((packed)) PCI_SELECT_DESC;

typedef struct
{
   __u32        dwInterrupt;
} __attribute__((packed)) INT_INFO;

#endif  /* ATEMSYS_H */
