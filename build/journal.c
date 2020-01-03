#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/module.h>	/* Specifically, a module */
#include <linux/fs.h>
#include <linux/uaccess.h>	/* for get_user and put_user */
#include <linux/slab.h>

#define MAJOR_NUM 0
#define SUCCESS 0
#define FAILURE -1
#define MAX_CHARACTERS 1500

MODULE_LICENSE("My License");
MODULE_AUTHOR("Guy Sudai / 1p4nm4n");
MODULE_DESCRIPTION("A short journal just for learning drivers");

/* Prototypes for device functions */
int   opened  = 0;
char* journal;
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

static int device_open(
		struct inode* inode, 
		struct file* file) {

   printk(KERN_INFO "device_open(%p, %p) has been executed!\n", inode, file);
   printk(KERN_INFO "The process id is %d\n", (int) task_pid_nr(current));
   //printk(KERN_INFO "The process vid is %d\n", (int) task_pid_vnr(current));
   printk(KERN_INFO "The process name is %s\n", current->comm);
   //printk("The process tty is %d\n", current->signal->tty);
   //printk(KERN_INFO "The process group is %d\n", (int) task_tgid_nr(current));
   if (opened) 
	   return FAILURE;

   opened++;
   return SUCCESS;
}

static int device_release(
		struct inode* inode, 
		struct file* file) {
   printk(KERN_INFO "device_release(%p, %p) has been executed!\n", inode, file);
   if (opened > 0) 
	   opened -= 1;
   return SUCCESS;
}


static ssize_t device_read(
		struct file* file_handle, 
		char* buffer, 
		size_t buf_length, 
		loff_t* offset) {

	char* tmp_journal = journal;
	int bytes_read = 0;
	printk(KERN_INFO "device_read(%p, %p, %ld) has been executed!\n", file_handle, buffer, buf_length);
	printk(KERN_INFO "journal(%ld, %p): %s\n", strlen(journal), journal, journal);	
	while (buf_length && *tmp_journal != '\0') {
		copy_to_user(buffer++, tmp_journal++, 1);
		buf_length--;
		bytes_read++;
	}

        printk(KERN_INFO "device_read(%p, %p, %ld) has returned %d bytes!\n", file_handle, buffer, buf_length, bytes_read);
	return bytes_read;
}

static ssize_t device_write(
		struct file* file_handle, 
		const char* buf, 
		size_t buf_length, 
		loff_t* offset) {
        printk(KERN_INFO "device_write(%p, %s, %ld) has been executed!\n", file_handle, buf, buf_length);
	printk(KERN_INFO "journal has %ld bytes available\n", strlen(journal));
	if (strlen(journal) + strlen(buf) < MAX_CHARACTERS) {
		char* tmp_journal = journal;
		while (*tmp_journal != '\0') {tmp_journal++;}
		while (*buf != '\0') {
			*(tmp_journal++) = *(buf++);
		}
		printk(KERN_INFO "Wrote %ld bytes into journal!", strlen(buf));
	} else {
		printk(KERN_ALERT "Ran out of space in the journal(%ld characters left)!", MAX_CHARACTERS - strlen(journal) - 1);
		return -1;
	}
	printk("journal(%ld, %p): %s", strlen(journal), journal, journal);

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
   	
	journal = (char*) kmalloc(MAX_CHARACTERS, sizeof(char));
   	memset(journal, '\0', MAX_CHARACTERS);
	if (journal != NULL) {
		printk(KERN_INFO "Allocated %d bytes for journal!", MAX_CHARACTERS);
	} else {
		printk(KERN_ALERT "Unable to allocate memory for journey(%d)", MAX_CHARACTERS);
		return FAILURE;
	}
	return SUCCESS;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Freeing journal(%ld, %p): %s from memory.", strlen(journal), journal, journal);
	kfree(journal);
	journal = NULL;
	printk(KERN_ALERT "Device journal has been shut down successfully!\n");
}

module_init(hello_init);
module_exit(hello_exit);
