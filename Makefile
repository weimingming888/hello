ifneq ($(KERNELRELEASE),)
# 内核构建阶段强制标准，优先级最高，覆盖外部所有CFLAGS
ccflags-y := -std=gnu11 -ffreestanding -fno-pie -fno-plt
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
        M=$(PWD)
else
        $(MAKE) -C $(KDIR) \
        ARCH=$(ARCH) \
        CROSS_COMPILE=$(CROSS_COMPILE) \
        M=$(PWD)
endif

clean:
        $(MAKE) -C $(KDIR) M=$(PWD) clean
endif
