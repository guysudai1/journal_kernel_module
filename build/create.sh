#!/bin/bash

make
insmod journal.ko

# Execute 
# sudo mknod /dev/journal c MAJOR_NUM 0
