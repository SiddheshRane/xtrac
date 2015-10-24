## -*- Makefile -*-
##
## User: siddhesh
## Time: 24 Oct, 2015 7:43:23 PM
## Makefile created by Oracle Solaris Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CC = gcc
BASICOPTS = -g
CFLAGS = $(BASICOPTS)


# Define the target directories.
TARGETDIR_xtrac=GNU-amd64-Linux


all: $(TARGETDIR_xtrac)/xtrac

## Target: xtrac
OBJS_xtrac =  \
	$(TARGETDIR_xtrac)/xtrac.o
USERLIBS_xtrac = $(SYSLIBS_xtrac) 
DEPLIBS_xtrac =  
LDLIBS_xtrac = $(USERLIBS_xtrac)


# Link or archive
$(TARGETDIR_xtrac)/xtrac: $(TARGETDIR_xtrac) $(OBJS_xtrac) $(DEPLIBS_xtrac)
	$(LINK.c) $(CFLAGS_xtrac) $(CPPFLAGS_xtrac) -o $@ $(OBJS_xtrac) $(LDLIBS_xtrac)


# Compile source files into .o files
$(TARGETDIR_xtrac)/xtrac.o: $(TARGETDIR_xtrac) xtrac.c
	$(COMPILE.c) $(CFLAGS_xtrac) $(CPPFLAGS_xtrac) -o $@ xtrac.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_xtrac)/xtrac \
		$(TARGETDIR_xtrac)/xtrac.o
	rm -f -r $(TARGETDIR_xtrac)


# Create the target directory (if needed)
$(TARGETDIR_xtrac):
	mkdir -p $(TARGETDIR_xtrac)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-amd64-Linux

