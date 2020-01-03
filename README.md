#Journal Kernel Module 
@ Project Write Kernel (module) Driver
@ Date: 1.1.2020
@ Author: Guy Sudai

Description: This kernel driver acts as a journal and can be written to or read from. 

#Installation 

1. Run `sudo build/remove.sh` (In order to remove the previous running kernel module).
2. Use `sudo build/create.sh` (In order to compile the journal.c file, and insmod the compiled kernel module).
3. Get major number from dmesg with `watch -c "dmesg | tail -n 15"`.
4. Run`sudo build/create_dev.sh` (In order to create a new /dev/journal char device and change its owner so it can be written to and read from)
5. Have fun.
