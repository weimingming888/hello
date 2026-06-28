#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("GPL"); // 必须GPL，否则加载报错
MODULE_AUTHOR("test");

static int __init mod_init(void)
{
    printk(KERN_INFO "Hello KO Module\n");
    return 0;
}
static void __exit mod_exit(void)
{
    printk(KERN_INFO "Exit KO Module\n");
}
module_init(mod_init);
module_exit(mod_exit);
