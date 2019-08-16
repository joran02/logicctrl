
include ../makefile.rule


MACRO_FLAG =  -DCHANNEL_8_MAC -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D_GNU_SOURCE



INC_PATH +=-I../../Hisi/include -I./ 
INC_PATH += -I../common

CFLAGS += -O0 -Wall   $(INC_PATH) $(MACRO_FLAG)

LIBDIR=../../ipc3519v101/lib

SRCS    :=  $(wildcard *.c)
OBJS    :=  $(SRCS:%.c=%.o)


SRCS2    :=  $(wildcard *.cpp)
OBJS2    :=  $(SRCS:%.cpp=%.o)

MY_OBJS := libtw.o

TARGET :=  libtw.so
.PHONY : clean all

#$(TARGET):$(OBJS)
$(TARGET):$(SRCS) $(SRCS2)
	$(CC)  -fPIC -shared  $(CFLAGS) -lpthread -lm  -o $@ $^ $(LIBS) 
	$(STRIP)  $(TARGET)
	cp $(TARGET) $(LIBDIR)
	
clean:
	@rm -f $(TARGET)
	@rm -f $(OBJS)


