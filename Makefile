PROG = acc_saxpy 
TARGET = $(PROG)
FINAL_TARGET = $(TARGET)
CC=pgcc
#CC=gcc

ifeq ($(CC),gcc)
	CFLAGS = -O3 -fopenacc
else
	CFLAGS = -O3 -acc -fast -Minfo

endif
LIBS = 
OBJS = acc_saxpy.o

all: $(FINAL_TARGET)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

run:
	./$(TARGET)

clean:
	rm -f $(PROG) *.o

