#!/bin/bash

mknod /dev/journal c $1 0
chmod u=rw,g=r,o=rw /dev/journal
