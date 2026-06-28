#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/fs.h>
#include <linux/nodemask.h>
#include <linux/mount.h>
#include <linux/path.h>
#include <linux/dcache.h>

// sys_mount 原型
struct sys_mount_args {
    const char __user *source;
    const char __user *target;
    const char __user *filesystem;
    unsigned long flags;
    void __user *data;
};

static struct kprobe kp_mount;

// 前置回调：进入 sys_mount 触发
static int pre_mount_handler(struct kprobe *p, struct pt_regs *regs)
{
    struct sys_mount_args args;
    char src_buf[256], tgt_buf[256], fstype_buf[128];
    long ret;

    // x86_64 寄存器传参：rdi rsi rdx rcx r8
    args.source = (void *)regs->di;
    args.target = (void *)regs->si;
    args.filesystem = (void *)regs->dx;

    // 拷贝用户态字符串
    ret = strncpy_from_user(src_buf, args.source, sizeof(src_buf)-1);
    if (ret < 0) return 0;
    ret = strncpy_from_user(tgt_buf, args.target, sizeof(tgt_buf)-1);
    if (ret < 0) return 0;
    ret = strncpy_from_user(fstype_buf, args.filesystem, sizeof(fstype_buf)-1);
    if (ret < 0) return 0;

    pr_info("[mount_hook] mount intercept: src=%s target=%s fstype=%s flags=0x%lx\n",
            src_buf, tgt_buf, fstype_buf, regs->cx);

    // 示例：拦截挂载 /tmp 目录，禁止挂载
    if (strstr(tgt_buf, "/tmp")) {
        pr_info("[mount_hook] BLOCK mount to /tmp\n");
        // 修改返回值，让 sys_mount 直接返回失败，跳过原函数执行
        regs->ax = -EPERM;
        kprobe_skip_handler(regs);
    }

    return 0;
}

static int __init mount_hook_init(void)
{
    kp_mount.symbol_name = "__x64_sys_mount";
    kp_mount.pre_handler = pre_mount_handler;

    if (register_kprobe(&kp_mount)) {
        pr_err("register kprobe sys_mount failed\n");
        return -1;
    }
    pr_info("mount hook loaded, hook __x64_sys_mount\n");
    return 0;
}

static void __exit mount_hook_exit(void)
{
    unregister_kprobe(&kp_mount);
    pr_info("mount hook unloaded\n");
}

module_init(mount_hook_init);
module_exit(mount_hook_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hook sys_mount example for Linux 6.1");
