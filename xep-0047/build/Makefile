.PHONY: all clean

XMPPLIB   = ../include/libstrophe/
OC_DIR = ../include/iotivity/resource/csdk

XMPPLIB_INC	= $(XMPPLIB)
XMPPLIB_INC_SRC = $(XMPPLIB)/src

INC_DIRS        += -I$(XMPPLIB_INC)
INC_DIRS        += -I$(XMPPLIB_INC_SRC)

SRCPATH = ../source
HDRPATH = ../include

BUILDPATH = ../build
OBJPATH = $(BUILDPATH)/obj
MIPS_OBJPATH = $(BUILDPATH)/obj_mips
BINPATH = $(BUILDPATH)/bin
LIBPATH = ../../

CC = gcc
LD = ld

CFLAGS =  -fPIC  -shared -O -Wall -fdata-sections -Wl,--gc-sections -Wl,-s -fno-exceptions -std=gnu99 $(INC_DIRS) 
#-L$(OC_DIR)/linux/$(BUILD)
DEFS = -DTB_LOG

INCLUDES = -I$(HDRPATH)
VPATH = $(SRCPATH)
SRCS=$(wildcard $(SRCPATH)/*.c) 

HEADERS=$(wildcard $(HDRPATH)/*.h)
NOTDIRSRCS=$(notdir $(SRCS))
OBJS = $(patsubst %.c,$(OBJPATH)/%.o,$(NOTDIRSRCS))
MIPS_OBJS= $(patsubst %.c,$(MIPS_OBJPATH)/%.o,$(NOTDIRSRCS))


#LIBS = $(LIBPATH)/libstrophe.a   $(LIBPATH)/libssl.a  $(LIBPATH)/libcrypto.a     $(LIBPATH)/libexpat.a -lpthread  -lm -lresolv -ldl

#LIBS = $(LIBPATH)/libstrophe.a $(LIBPATH)/libexpat.a -lpthread  -lssl -lcrypto  -lm -lresolv 
EXAMPLE_PATH=../example

LIBS = $(LIBPATH)/libstrophe.a $(LIBPATH)/libexpat.a -lpthread  -lssl -lcrypto  -lm -lresolv 


all: libxep0047.a xep0047-test

xep0047-test: libxep0047.a
	$(CC) $(CFLAGS) -c  -o  $(BINPATH)/main.o   $(EXAMPLE_PATH)/main.c   $(INCLUDES)     
	$(CC) -z muldefs  -o  $(BINPATH)/$@   $(BINPATH)/main.o  $^ $(LIBS)	
	cp  $(BINPATH)/$@ $(LIBPATH)/$@
	@echo Compile XEP-0047 test program successful....

libxep0047.a : $(OBJS)
	@mkdir -p $(BINPATH)
	ar cru   $@ $^ 
	@cp $@ $(LIBPATH)/$@
	@echo Compile libxep0047.a successful....

$(OBJS): $(OBJPATH)/%.o: %.c
	@mkdir -p $(OBJPATH)
	$(CC) $(CFLAGS) -c -o $@  $< $(INCLUDES) $(DEFS)

clean:
	rm -rf $(BINPATH) $(OBJPATH) $(LIBPATH)/libxep0047.a  $(LIBPATH)/xep0047-test *.a
