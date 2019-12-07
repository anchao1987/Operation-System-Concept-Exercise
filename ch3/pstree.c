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
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/sched/signal.h>

#define BUFFER_SIZE 128
#define PROC_NAME "pstree"

/**
 * Function prototypes
 * Notice for_each_process macro has moved to <linux/sched/signal.h> ref:
 */
static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static void printPsTree(struct task_struct *head);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read};

/* This function is called when the module is loaded. */
static int proc_init(void)
{
        // creates the /proc/procfs entry
        proc_create(PROC_NAME, 0666, NULL, &proc_ops);

        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

        return 0;
}

/* This function is called when the module is removed. */
static void proc_exit(void)
{
        // removes the /proc/procfs entry
        remove_proc_entry(PROC_NAME, NULL);

        printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/pid is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 */
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        static int completed = 0;

        if (completed)
        {
                completed = 0;
                return 0;
        }

        completed = 1;
        printk(KERN_INFO "Progress Info:\n");

        printPsTree(&init_task);

        return 0;
}

static void printPsTree(struct task_struct *head)
{
        struct list_head *list;
        struct task_struct *task;
        printk(KERN_INFO "%s  %ld %d\n", head->comm, head->state, head->pid);

        if (list_empty(&(head->children)) == 1)
        {
                return;
        }

        list_for_each(list, &head->children)
        {
                task = list_entry(list, struct task_struct, sibling);
                printPsTree(task);

                /* task points to the next child in the list */
        }
}

/* Macros for registering module entry and exit points. */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module");
MODULE_AUTHOR("SGG");
