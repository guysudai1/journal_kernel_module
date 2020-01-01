#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/module.h>	/* Specifically, a module */
#include <linux/fs.h>
#include <linux/uaccess.h>	/* for get_user and put_user */

#define MAJOR_NUM 0
#define SUCCESS 0
#define FAILURE -1

MODULE_LICENSE("My License");
MODULE_AUTHOR("Guy Sudai / 1p4nm4n");
MODULE_DESCRIPTION("A short journal just for learning drivers");

/* Prototypes for device functions */

const char* my_string = "hello world 12345";
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

struct file_operations file_ops = {
 .read = device_read,
 .write = device_write,
 .open = device_open,
 .release = device_release
};

static int device_open(struct inode* inode, struct file* file) {
   printk(KERN_INFO "device_open(%p, %p) has been executed!\n", inode, file);
   printk(KERN_INFO "The process id is %d\n", (int) task_pid_nr(current));
   printk("The process vid is %d\n", (int) task_pid_vnr(current));
   printk("The process name is %s\n", current->comm);
   //printk("The process tty is %d\n", current->signal->tty);
   printk("The process group is %d\n", (int) task_tgid_nr(current));
   return SUCCESS;
}

static int device_release(struct inode* inode, struct file* file) {
   my_string = "hello world 12345";
   printk(KERN_INFO "device_release(%p, %p) has been executed!\n", inode, file);
   return SUCCESS;
}


static ssize_t device_read(struct file* file_handle, char* buffer, size_t buf_length, loff_t* offset ) {
	int bytes_read = 0;
	int my_length = strlen(my_string);
	printk(KERN_INFO "device_read(%p, %p, %ld) has been executed!\n", file_handle, buffer, buf_length);
	while (buf_length && bytes_read < my_length) {
		copy_to_user(buffer++, my_string++, 1);
		buf_length--;
		bytes_read++;
	}

        printk(KERN_INFO "device_read(%p, %p, %ld) has returned %d bytes!\n", file_handle, buffer, buf_length, bytes_read);
	return bytes_read;
}

static ssize_t device_write(struct file* file_handle, const char* buf, size_t buf_length, loff_t* offset) {
        printk(KERN_INFO "device_write(%p, %s, %ld) has been executed!\n", file_handle, buf, buf_length);
	return buf_length;
}

static int hello_init(void)
{

	int retval;
	if ((retval = register_chrdev(MAJOR_NUM, "journal", &file_ops)) < 0) {
		printk(KERN_ALERT "Device %s registration failed! Error received: %d\n",
			"journal", retval
		);
		return retval;
	
	}

	printk(KERN_INFO "Successfully registered chr device, major number allocated: %d\n", retval);
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Shut down Device Journal\n");
}

module_init(hello_init);
module_exit(hello_exit);
