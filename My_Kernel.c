#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <asm/current.h>

#define procfs_name "Mythread_info"
#define BUFSIZE  1024
char buf[BUFSIZE];

static ssize_t Mywrite(struct file *fileptr, const char __user *ubuf, size_t buffer_len, loff_t *offset){
    /* Do nothing */
	return 0;
}


static ssize_t Myread(struct file *fileptr, char __user *ubuf, size_t buffer_len, loff_t *offset){
    /*Your code here*/
    int len;
    if (*offset > 0)  // 如果偏移量大于 0，表示已经读取过数据，返回 0 结束读取
        return 0;

    len = snprintf(buf, BUFSIZE, "Current Process ID: %d\n", current->pid);  // 格式化进程 ID
    if (copy_to_user(ubuf, buf, len))  // 将数据从内核空间复制到用户空间
        return -EFAULT;  // 如果失败，返回错误码

    *offset += len;  // 更新偏移量
    return len;  // 返回实际读取的字节数
    /****************/
}

static struct proc_ops Myops = {
    .proc_read = Myread,
    .proc_write = Mywrite,
};

static int My_Kernel_Init(void){
    proc_create(procfs_name, 0644, NULL, &Myops);   
    pr_info("My kernel says Hi");
    return 0;
}

static void My_Kernel_Exit(void){
    pr_info("My kernel says GOODBYE");
}

module_init(My_Kernel_Init);
module_exit(My_Kernel_Exit);

MODULE_LICENSE("GPL");