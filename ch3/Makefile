obj-m += pid.o
obj-m += ps.o
obj-m += pstree.o
obj-m += colorlist.o
obj-m += collatzmod.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
