#obj-m = module7.o
#KVERSION = $(shell uname -r)
#all:
#        make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
#clean:
#        make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean



obj-m +=module1.o

KDIR = /usr/src/linux-headers-$(shell uname -r)


all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order *~


	
