# VARIABLES #
#compiler specific variables <for ON-FLY compiler settings use the OPT variable>
CC	= gcc
CFLAGS	= -ggdb3 -pg -Wall -Wextra -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wconversion -Wmissing-declarations -Wunreachable-code -Winline
OPT	+=

#header files, libraries, paths ...
INC_PATH= ../inc
INC_FLAG= -I$(INC_PATH)
COMM_DEP= $(INC_PATH)/%.h $(INC_PATH)/common.h $(LIB_PATH)/common.c

LIB_PATH= ../lib
LIB_FLAG= -L$(LIB_PATH)
INS_PATH= 

LIBS	= -largtable2 -lfftw3 -lfann -lm -lrt -lcorti

#objects, dependencies ... <ONLY FILES or file wildcards are allowed in TAGS variable>
TAGS	+= ./*.[ch] $(INC_PATH)/*.h $(LIB_PATH)/*.c

OBJ 	= $(LIB_PATH)/fft.o $(LIB_PATH)/aiff.o $(LIB_PATH)/window.o $(LIB_PATH)/ann.o $(LIB_PATH)/common.o

LIBRARY	= $(LIB_PATH)/libcorti.a

PROG	= corti

# RULES #
#declare PHONY rules
.PHONY	: global_default local_default clean distclean cleantags all cleanall install uninstall

#default rule
global_default: local_default

#tags rules
tags	: $(TAGS)
	@echo -e "\033[00;32mbuilding tags [$(TAGS)]\033[00m"
	@echo "ctags $(TAGS)"
	@ctags $(TAGS)\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"
	@echo ""

cleantags:
	@echo -e "\033[00;32mcleaning tags\033[00m"
	@echo "rm -f tags"
	@rm -f tags\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"
	@echo ""

#rules for complete rebuild
all	: cleanall global_default

cleanall: clean cleantags

#default rule is trigered only for missing targets
.DEFAULT:
	@echo -e "\033[00;31mNo rule to make target '$@'\033[00m"
	@exit 1
