CROSS_COMPILE = /usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-
#CROSS_COMPILE = arm-none-linux-gnueabi-

AS      =   $(CROSS_COMPILE)as
LD      =   $(CROSS_COMPILE)ld
CC      =   $(CROSS_COMPILE)gcc
CPP     =   $(CC)   -E
AR      =   $(CROSS_COMPILE)ar
NM      =   $(CROSS_COMPILE)nm
STRIP   =   $(CROSS_COMPILE)strip
OBJCOPY =   $(CROSS_COMPILE)objcopy
OBJDUMP =   $(CROSS_COMPILE)objdump


export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP

# 编译器的参数设置，显示所有警告、优化等级、调试信息
CFLAGS := -Wall -O2 -g -DDEBUG

CFLAGS += -I $(shell pwd)/include -I/opt/libdecode/include/

LDFLAGS := -ljpeg -lz -lpng -L/opt/libdecode/lib

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

# 定义最终生成的可执行程序的名字
TARGET := imagePlayer


# 添加项目中所有用到的源文件，顶层目录下的.c文件，和子文件夹
obj-y += main.o
# 添加顶层目录下的子文件夹（注意结尾的/）
obj-y += display/
obj-y += image_manage/

all:
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS)  -o $(TARGET) built-in.o
    
cp:
	cp imagePlayer /root/porting_x210/rootfs/rootfs/usr/testProject -rf
cpall:
	cp ../testProject /root/porting_x210/rootfs/rootfs/usr/ -rf

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(TARGET)
    
distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)

