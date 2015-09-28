CC = gcc
CFLAGS = -Wall
TARGET = xtrac

all : xtrac.o
	$(CC) $(CFLAGS) xtrac.o -o $(TARGET)

xtrac.o : xtrac.c
	$(CC) $(CFLAGS) -c xtrac.c -o xtrac.o
	
clean : $(RM) *.o 
