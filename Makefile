VPATH=src/main
VPATH += :src/conf
VPATH += :src/fs
VPATH += :src/common
vpath %.h inc inc/main inc/fs inc/common


Objects += main.o
Objects += monitor.o
Objects += common.o

CROSS_COMPILE=arm-linux-gnueabihf-
AS=$(CROSS_COMPILE)as
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
NM= $(CROSS_COMPILE)nm
SIZE=$(CROSS_COMPILE)size
OBJCOPY=$(CROSS_COMPILE)objcopy
OBJDUMP=$(CROSS_COMPILE)objdump



#CFlag=-I./inc -I./inc/main -I./inc/fs
CFLAGS+=-I./inc -I./inc/main -I./inc/fs -I./inc/common


TARGET=fs_monitor
#LIBS=-lpthread -lm
LIBS +=-lpthread -lm
LDFLAGS=-L.


$(TARGET):$(Objects)
	#lpr -p $?  gcc -M main.c
	#$(CC)  $(CFlag) $(OSOINCLUDES) $(LIBS)  $(LDFLAGS) -o $@ $(Objects)    
	$(CC)  $(CFlag) $(OSOINCLUDES) -o $@ $(Objects) $(LIBS)

main.o:main.h
monitor.o:monitor.h
common.o:common.h
.PHONY : clean
clean :
	-rm fs_monitor $(Objects)
