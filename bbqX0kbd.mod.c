#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x49cd25ed, "alloc_workqueue" },
	{ 0x933d25b9, "devm_kmalloc" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x8813189a, "devm_input_allocate_device" },
	{ 0x9d669763, "memcpy" },
	{ 0x92997ed8, "_printk" },
	{ 0x907df9fd, "input_register_device" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xcf86cdac, "queue_delayed_work_on" },
	{ 0xb2e63650, "i2c_register_driver" },
	{ 0x5dc02b57, "_dev_err" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0xceccfca7, "input_set_capability" },
	{ 0x5bf77469, "_dev_warn" },
	{ 0x9166fc03, "__flush_workqueue" },
	{ 0xb2541cf3, "i2c_smbus_read_word_data" },
	{ 0x6808df33, "input_event" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0x474fe39f, "i2c_smbus_write_byte_data" },
	{ 0xcbd7de6d, "i2c_del_driver" },
	{ 0x412e773, "i2c_smbus_read_byte_data" },
	{ 0xf9a482f9, "msleep" },
	{ 0xbbb36cd0, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*CwallComputer,bbqX0kbd");
MODULE_ALIAS("of:N*T*CwallComputer,bbqX0kbdC*");
MODULE_ALIAS("i2c:bbqX0kbd");

MODULE_INFO(srcversion, "52F0E337858DF1987ED3AA7");
