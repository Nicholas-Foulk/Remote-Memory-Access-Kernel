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
	{ 0xef025c67, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x75f2af9e, __VMLINUX_SYMBOL_STR(sock_release) },
	{ 0x39c43cf2, __VMLINUX_SYMBOL_STR(sock_recvmsg) },
	{ 0x94745f80, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x582bcf5, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x2d693c23, __VMLINUX_SYMBOL_STR(sock_create_lite) },
	{ 0x5ca2cb29, __VMLINUX_SYMBOL_STR(sock_create_kern) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "305DA3D9C9BA12AE0522591");
