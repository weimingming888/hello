ifneq ($(KERNELRELEASE),)
# Kbuild阶段：声明编译模块
obj-m += hello.o
else
# 外部make入口，变量由Action传入
KDIR ?= ./kernel
ARCH ?= x86_64
CROSS_COMPILE ?= 
LLVM ?= 0
PWD := $(shell pwd)

all:
ifeq ($(LLVM),1)
	$(MAKE) -C $(KDIR) ARCH=$(ARCH) LLVM=1 M=$(PWD) modules
else
	$(MAKE) -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules
endif

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
endif
