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
#include <linux/moduleparam.h>
#define PROC_NAME "collatz"

/* the current pid */
struct collatz
{
        int value;
        struct list_head list;
};

static LIST_HEAD(collatz_list);
static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
};
static int start = 25;
module_param(start, int, 0);

/* This function is called when the module is loaded. */
static int proc_init(void)
{
        struct collatz *cl;
        struct collatz *ptr;
        // creates the /proc/procfs entry
        proc_create(PROC_NAME, 0666, NULL, &proc_ops);

        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
        while (start != 1)
        {

                cl = kmalloc(sizeof(*cl), GFP_KERNEL);
                cl->value = start;

                INIT_LIST_HEAD(&cl->list);
                list_add_tail(&cl->list, &collatz_list);
                if (start % 2 == 0)
                {
                        start = start / 2;
                }
                else
                {
                        start = 3 * start + 1;
                }
        }
        cl = kmalloc(sizeof(*cl), GFP_KERNEL);
        cl->value = start;

        INIT_LIST_HEAD(&cl->list);
        list_add_tail(&cl->list, &collatz_list);

        list_for_each_entry(ptr, &collatz_list, list)
        {
                printk(KERN_INFO "%d ", ptr->value);
        }
        printk(KERN_INFO "\n");

        return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void)
{
        struct collatz *ptr, *next;
        list_for_each_entry_safe(ptr, next, &collatz_list, list)
        {
                printk(KERN_INFO "Remove Collatz Sequence: %d\n", ptr->value);

                /* on each iteration ptr points */
                /* to the next struct collatz */
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
