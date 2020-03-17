# kernel-challenge
T884 Task-1 
There are seperate modules for PROC operations and ioctl operations for the sake of clarity

1.First we add the proc module

clean the direcory using


 make -C /lib/modules/$(uname -r)/build M=$PWD clean

Add the module sourcefile:

 make -C /lib/modules/$(uname -r)/build M=$PWD modules

 insert the module:

insmod simproc.ko

2.Seocnd we add the ioctl module

make -C /lib/modules/$(uname -r)/build M=$PWD clean

make -C /lib/modules/$(uname -r)/build M=$PWD modules

insmod driver.ko

3.Add the main character device driver

make -C /lib/modules/$(uname -r)/build M=$PWD clean

make -C /lib/modules/$(uname -r)/build M=$PWD modules

insmod chardvr.ko

For compiling the driver test file

gcc chartest.c -o chrdev_test

For creating a character special file

NOTE: First know the major number

grep chrdev /proc/devices

and then

mknod chrdev c <major number> 0

Finally:

./chrdev_test chrdev

