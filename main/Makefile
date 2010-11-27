include ../Makefile.inc

#default rule
local_default: $(PROG) tags

#program rules
$(PROG)	: %: %.c $(LIBRARY) $(COMM_DEP) 
	@echo -e "\033[00;32mbuilding $@\033[00m"
	@echo "$(CC) $(INC_FLAG) $(LIB_FLAG) $(CFLAGS) $(OPT) $< $(LIBS) -o $@"
	@$(CC) $(INC_FLAG) $(LIB_FLAG) $(CFLAGS) $(OPT) $< $(LIBS) -o $@\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"
	@echo ""

$(LIBRARY): $(OBJ)
	@( cd $(LIB_PATH); $(MAKE) | sed -e "/^[ 	]*[^ 	]/s/^/"`echo -e "\033[00;32m$(LIB_PATH):\033[00m" | sed -e "s/\//\\\\\\\\\\\//"`"	/" ) | sed -e "1 d; $$ d"\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"

%.o	: %.c $(COMM_DEP)
	@( cd $(LIB_PATH); $(MAKE) $(@F) | sed -e "/^[ 	]*[^ 	]/s/^/"`echo -e "\033[00;32m$(LIB_PATH):\033[00m" | sed -e "s/\//\\\\\\\\\\\//"`"	/" ) | sed -e "1 d; $$ d"\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"

clean	:
	@echo -e "\033[00;32mcleaning $(PROG)\033[00m"
	@echo "rm -f $(PROG)"
	@rm -f $(PROG)\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"
	@echo ""

#installation rules
install	: $(PROG)
	@echo -e "\033[00;32minstaling $(PROG)\033[00m"
	@echo "cp -vf $(PROG) $(INS_PATH)/$(PROG)"
	@if [ -n "$(INS_PATH)" -a -d "$(INS_PATH)" ]; then\
			cp -vf $(PROG) $(INS_PATH)/$(PROG);\
		else\
			echo "'$(INS_PATH)' is empty or not a directory" >&2;\
		fi\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"
	@echo ""

uninstall:
	@echo -e "\033[00;32muninstalling $(PROG)\033[00m"
	@echo "rm -vf $(INS_PATH)/$(PROG)"
	@if [ -n "$(INS_PATH)" -a -d "$(INS_PATH)" ]; then\
			rm -vf $(INS_PATH)/$(PROG);\
		else\
			echo "'$(INS_PATH)' is empty or not a directory" >&2;\
		fi\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"
	@echo ""
