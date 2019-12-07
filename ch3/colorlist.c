/**
 * Kernel module that communicates with /proc file system.
 * 
 * This provides the base logic for Project 2 - displaying task information
 * install mode: sudo insmod pid.ko
 * check kernel mode message: dmesg
 * remove mode: sudo rmmod pid
 */

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/types.h>
#include <linux/list.h>

#define BUFFER_SIZE 128
#define PROC_NAME "colorlist"

/* the current pid */
struct color
{
        int red;
        int blue;
        int green;
        struct list_head list;
};

static LIST_HEAD(color_list);
static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
};

/* This function is called when the module is loaded. */
static int proc_init(void)
{
        struct color *violet;
        struct color *ptr;
        int i = 0;
        // creates the /proc/procfs entry
        proc_create(PROC_NAME, 0666, NULL, &proc_ops);

        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

        for (; i < 4; i++)
        {
                violet = kmalloc(sizeof(*violet), GFP_KERNEL);
                violet->red = 138 + i;
                violet->blue = 43 + i;
                violet->green = 226 + i;

                INIT_LIST_HEAD(&violet->list);
                list_add_tail(&violet->list, &color_list);
        }

        list_for_each_entry(ptr, &color_list, list)
        {
                printk(KERN_INFO "Add (R:%d,G:%d,B:%d)\n", ptr->red, ptr->green, ptr->blue);
        }
        return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void)
{
        struct color *ptr, *next;
        list_for_each_entry_safe(ptr, next, &color_list, list)
        {
                printk(KERN_INFO "Remove (R:%d,G:%d,B:%d)\n", ptr->red, ptr->green, ptr->blue);

                /* on each iteration ptr points */
                /* to the next struct color */
                list_del(&ptr->list);
                kfree(ptr);
        }
        // removes the /proc/procfs entry
        remove_proc_entry(PROC_NAME, NULL);

        printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/* Macros for registering module entry and exit points. */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module");
MODULE_AUTHOR("SGG");
