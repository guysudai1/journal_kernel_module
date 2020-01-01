#!/bin/bash

echo "Removing journal from lsmod..."
if rmmod journal >/dev/null; then
	echo "Removed journal from module list successfully!"
else
	echo "Unable to remove journal module from module list!"
fi

echo ""

echo "Attempting to remove journal device file..."
if rm /dev/journal >/dev/null; then
	echo "Removed journal device file successfully!"
else
	echo "Unable to remove journal device file!"
fi

echo "Closing program..."
