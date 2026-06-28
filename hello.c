#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// 模块信息
MODULE_LICENSE("GPL");
MODULE_AUTHOR("test");
MODULE_DESCRIPTION("Simple test ko module");
MODULE_VERSION("1.0");

// 模块加载入口
static int __init hello_init(void)
{
    pr_info("Hello Kernel Module Loaded!\n");
    return 0;
}

// 模块卸载入口
static void __exit hello_exit(void)
{
    pr_info("Hello Kernel Module Unloaded!\n");
}

module_init(hello_init);
module_exit(hello_exit);