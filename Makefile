#### Compiler and tool definitions shared by all build targets #####
CC = gcc
CFLAGS = -c

all: project

## Target: project
OBJS =  \
	main.o \
	xtrac.o \
	dextr.o \
	list.o

# Link
project:  $(OBJS)
	$(CC) -o $@ $(OBJS) 

# Compile source files into .o files
main.o:  main.c
	$(CC) $(CFLAGS)  -o $@ main.c

xtrac.o:  xtrac.c
	$(CC) $(CFLAGS)  -o $@ xtrac.c

dextr.o:  dextr.c
	$(CC) $(CFLAGS)  -o $@ dextr.c

list.o:  list.c
	$(CC) $(CFLAGS)  -o $@ list.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		project \
		main.o \
		xtrac.o \
		dextr.o \
		list.o


