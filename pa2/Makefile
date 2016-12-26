COMPILER = gcc
CCFLAGS= -g -Wall
LIB = libsl.a
PROGRAM = sl

all: $(PROGRAM)
$(PROGRAM): main.o $(LIB)
	$(COMPILER) $(CCFLAGS) -o $(PROGRAM) main.o
main.o: main.c
	$(COMPILER) $(CCFLAGS) -c main.c 
$(LIB): sorted-list.o
	ar  rcs $(LIB)      sorted-list.o
sorted-list.o: sorted-list.c sorted-list.h
	$(COMPILER) $(CCFLAGS) -c sorted-list.c
clean:
	rm -f *.o *.a $(PROGRAM)