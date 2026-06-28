ifneq ($(KERNELRELEASE),)
obj-m += hello.o
else
KDIR ?= ./kernel
ARCH ?= arm64
CROSS_COMPILE ?=
LLVM ?= 0
PWD := $(shell pwd)

all:
ifeq ($(LLVM),1)
	$(MAKE) -C $(KDIR) \
	ARCH=$(ARCH) \
	CC=clang \
	CXX=clang++ \
	LD=ld.lld \
	LLVM=1 \
	M=$(PWD) modules
else
	$(MAKE) -C $(KDIR) \
	ARCH=$(ARCH) \
	CROSS_COMPILE=$(CROSS_COMPILE) \
	M=$(PWD) modules
endif

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
endif
