#include <linux/sched.h>
#include <linux/rcupdate.h>
#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <asm/siginfo.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/rcupdate.h>
#include <linux/string.h>
 
MODULE_LICENSE("GPL");
 
 
#define PROCFS_MAX_SIZE         1024
#define PROCFS_TESTLEVEL        "battery_test"
#define PROCFS_NOTIFYPID        "battery_notify"
#define PROCFS_THRESHOLD        "battery_threshold"
 
 
/* Declaration of variables used in this module */
 
 
static int level = 99;
static int test_level = 0;                      //indicates level of battery remain.
static int notify_pid = -1;
static int threshold = -1;
 
/* End of declaration */
 
 
 
/* Declaration of ancillary variables */
 
static char procfs_buffer[PROCFS_MAX_SIZE];     
static unsigned long procfs_buffer_size = 0;    //size of receive side buffer
static struct proc_dir_entry *proc_test_level;       //indicates procfs entry.
static struct proc_dir_entry *proc_notify_pid;       //indicates procfs entry.
static struct proc_dir_entry *proc_threshold;       //indicates procfs entry.

/* End of declaration */
 
 
 
/*
        Implementation of procfs write function
*/
static int test_level_write( struct file *filp, const char *user_space_buffer, unsigned long len, loff_t *off )
{
 
        int status = 0;
        int requested;
 
        procfs_buffer_size = len;
 
        if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
                procfs_buffer_size = PROCFS_MAX_SIZE;
        }
 
        /* write data to the buffer */
        if ( copy_from_user(procfs_buffer, user_space_buffer, procfs_buffer_size) ) {
                return -EFAULT;
        }
 
        status  = kstrtoint(procfs_buffer, 10, &requested);
        if(status < 0)
        {
                printk(KERN_INFO "Error while called kstrtoint(...)\n");
                return -ENOMEM;
        }
        // validate level value.
        if(requested< 0 || requested > 100){
                printk(KERN_INFO "Invalid battery level.\n");
                return -ENOMEM;
        }
        // accept value.
        test_level = requested;
 
        // *off += procfs_buffer_size; // not necessary here!
 
        return procfs_buffer_size;
 
}
 
/*
        Implementation of procfs read function
*/
static int test_level_read( struct file *filp, char *user_space_buffer, size_t count, loff_t *off )
{
        int ret = 0;
        int flag = 0;
 
        if(*off < 0) *off = 0;
 
        snprintf(procfs_buffer, 16, "%d\n", test_level);
        procfs_buffer_size = strlen(procfs_buffer);
 
        if(*off > procfs_buffer_size){
                return -EFAULT;
        }else if(*off == procfs_buffer_size){
                return 0;
        }
 
        if(procfs_buffer_size - *off > count)
                ret = count;
        else
                ret = procfs_buffer_size - *off;
 
 
 
 
        flag = copy_to_user(user_space_buffer, procfs_buffer + (*off), ret);
 
        if(flag < 0)
                return -EFAULT;
 
        *off += ret;
 
        return ret;
 
}
 
 
static int notify_pid_write( struct file *filp, const char *user_space_buffer, unsigned long len, loff_t *off )
{
 
        int status = 0;
        int requested;
 
        procfs_buffer_size = len;
 
        if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
                procfs_buffer_size = PROCFS_MAX_SIZE;
        }
 
        /* write data to the buffer */
        if ( copy_from_user(procfs_buffer, user_space_buffer, procfs_buffer_size) ) {
                return -EFAULT;
        }
 
        status  = kstrtoint(procfs_buffer, 10, &requested);
        if(status < 0)
        {
                printk(KERN_INFO "Error while called kstrtoint(...)\n");
                return -ENOMEM;
        }
        // accept value.
        test_level = requested;
 
        // *off += procfs_buffer_size; // not necessary here!
 
        return procfs_buffer_size;

}


static int notify_pid_read( struct file *filp, char *user_space_buffer, size_t count, loff_t *off )
{
        int ret = 0;
        int flag = 0;
 
        if(*off < 0) *off = 0;
 
        snprintf(procfs_buffer, 16, "%d\n", test_level);
        procfs_buffer_size = strlen(procfs_buffer);
 
        if(*off > procfs_buffer_size){
                return -EFAULT;
        }else if(*off == procfs_buffer_size){
                return 0;
        }
 
        if(procfs_buffer_size - *off > count)
                ret = count;
        else
                ret = procfs_buffer_size - *off;
 
 
 
 
        flag = copy_to_user(user_space_buffer, procfs_buffer + (*off), ret);
 
        if(flag < 0)
                return -EFAULT;
 
        *off += ret;
 
        return ret;
 
}


static int threshold_write( struct file *filp, const char *user_space_buffer, unsigned long len, loff_t *off )
{
 
        int status = 0;
        int requested;
 
        procfs_buffer_size = len;
 
        if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
                procfs_buffer_size = PROCFS_MAX_SIZE;
        }
 
        /* write data to the buffer */
        if ( copy_from_user(procfs_buffer, user_space_buffer, procfs_buffer_size) ) {
                return -EFAULT;
        }
 
        status  = kstrtoint(procfs_buffer, 10, &requested);
        if(status < 0)
        {
                printk(KERN_INFO "Error while called kstrtoint(...)\n");
                return -ENOMEM;
        }
        // accept value.
        test_level = requested;
 
        // *off += procfs_buffer_size; // not necessary here!
 
        return procfs_buffer_size;

}


static int threshold_read( struct file *filp, char *user_space_buffer, size_t count, loff_t *off )
{
        int ret = 0;
        int flag = 0;
 
        if(*off < 0) *off = 0;
 
        snprintf(procfs_buffer, 16, "%d\n", test_level);
        procfs_buffer_size = strlen(procfs_buffer);
 
        if(*off > procfs_buffer_size){
                return -EFAULT;
        }else if(*off == procfs_buffer_size){
                return 0;
        }
 
        if(procfs_buffer_size - *off > count)
                ret = count;
        else
                ret = procfs_buffer_size - *off;
 
 
 
 
        flag = copy_to_user(user_space_buffer, procfs_buffer + (*off), ret);
 
        if(flag < 0)
                return -EFAULT;
 
        *off += ret;
 
        return ret;
 
}


/*
        Configuration of file_operations
 
        This structure indicate functions when read or write operation occured.
*/
static const struct file_operations proc_test_level_fops = {
        .write = test_level_write,
        .read = test_level_read,
};

static const struct file_operations proc_notify_pid_fops = {
        .write = notify_pid_write,
        .read = notify_pid_read,
};
 
static const struct file_operations proc_threshold_fops = {
        .write = threshold_write,
        .read = threshold_read,
};
 
/*
        This function will be called on initialization of  kernel module
*/
int init_module(void)
{
 
        int ret = 0;
 
        proc_test_level = proc_create(PROCFS_TESTLEVEL, 0666, NULL, &proc_test_level_fops);
 
        if(proc_test_level == NULL)
        {
                return -ENOMEM;
        }

        proc_notify_pid = proc_create(PROCFS_NOTIFYPID, 0666, NULL, &proc_notify_pid_fops);
 
        if(proc_notify_pid == NULL)
        {
                return -ENOMEM;
        }

        proc_threshold = proc_create(PROCFS_THRESHOLD, 0666, NULL, &proc_threshold_fops);
 
        if(proc_threshold == NULL)
        {
                return -ENOMEM;
        }
        return ret;
 
}
 
/*
        This function will be called on cleaning up of kernel module
*/
void cleanup_module(void)
{
        remove_proc_entry(PROCFS_TESTLEVEL, proc_test_level);
        remove_proc_entry(PROCFS_NOTIFYPID, proc_notify_pid);
        remove_proc_entry(PROCFS_THRESHOLD, proc_threshold);
}

MODULE_LICENSE("GPL");
module_init(init_module);
module_exit(cleanup_module);
