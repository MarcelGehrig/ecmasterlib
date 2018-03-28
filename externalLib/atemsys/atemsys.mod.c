#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x77418c7c, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0xb9f0e03a, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xa31376ab, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0xb12bcea6, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xbe71965b, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x98afcf26, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0xbe09b254, __VMLINUX_SYMBOL_STR(try_module_get) },
	{ 0x16305289, __VMLINUX_SYMBOL_STR(warn_slowpath_null) },
	{ 0x93fca811, __VMLINUX_SYMBOL_STR(__get_free_pages) },
	{ 0x78764f4e, __VMLINUX_SYMBOL_STR(pv_irq_ops) },
	{ 0x70039a18, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x5720843c, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xfb578fc5, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x254565ca, __VMLINUX_SYMBOL_STR(dma_ops) },
	{ 0xeca4e404, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0xc715d9e0, __VMLINUX_SYMBOL_STR(boot_cpu_data) },
	{ 0xb419b20d, __VMLINUX_SYMBOL_STR(pci_enable_msi_block) },
	{ 0x1649b2fd, __VMLINUX_SYMBOL_STR(pci_set_master) },
	{ 0x784fb75f, __VMLINUX_SYMBOL_STR(dma_supported) },
	{ 0xa84691a1, __VMLINUX_SYMBOL_STR(dma_set_mask) },
	{ 0x4d49e14a, __VMLINUX_SYMBOL_STR(pci_try_set_mwi) },
	{ 0xdc374042, __VMLINUX_SYMBOL_STR(pci_get_device) },
	{ 0x49720e50, __VMLINUX_SYMBOL_STR(pci_request_regions) },
	{ 0xea214259, __VMLINUX_SYMBOL_STR(pci_enable_device) },
	{ 0xe825a376, __VMLINUX_SYMBOL_STR(pci_get_domain_bus_and_slot) },
	{ 0x6729d3df, __VMLINUX_SYMBOL_STR(__get_user_4) },
	{ 0xb2fd5ceb, __VMLINUX_SYMBOL_STR(__put_user_4) },
	{ 0x4c4fef19, __VMLINUX_SYMBOL_STR(kernel_stack) },
	{ 0x2072ee9b, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0xb3397100, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xb3889c24, __VMLINUX_SYMBOL_STR(_mutex_unlock) },
	{ 0xafb3382c, __VMLINUX_SYMBOL_STR(_mutex_lock) },
	{ 0x6d28373b, __VMLINUX_SYMBOL_STR(module_put) },
	{ 0x29536e47, __VMLINUX_SYMBOL_STR(pci_disable_msi) },
	{ 0x2e18ac5e, __VMLINUX_SYMBOL_STR(pci_release_regions) },
	{ 0xb844024e, __VMLINUX_SYMBOL_STR(pci_clear_master) },
	{ 0x455c81a3, __VMLINUX_SYMBOL_STR(pci_disable_device) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xfcec0987, __VMLINUX_SYMBOL_STR(enable_irq) },
	{ 0x1000e51, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0xff00e9e, __VMLINUX_SYMBOL_STR(prepare_to_wait) },
	{ 0xb073a5b2, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0xc8b57c27, __VMLINUX_SYMBOL_STR(autoremove_wake_function) },
	{ 0xfe5d930b, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x4302d0eb, __VMLINUX_SYMBOL_STR(free_pages) },
	{ 0x4c9d28b0, __VMLINUX_SYMBOL_STR(phys_base) },
	{ 0x200ee5a2, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xf20dabd8, __VMLINUX_SYMBOL_STR(free_irq) },
	{ 0x27bbf221, __VMLINUX_SYMBOL_STR(disable_irq_nosync) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "BE775D3ED13C717BD565FE9");
