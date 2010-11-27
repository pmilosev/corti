include ../Makefile.inc

L_OBJ	= $(foreach file, $(OBJ), 
#default rule
local_default: $(LIBRARY) tags

#program rules
$(LIBRARY) : $(OBJ)
	@echo -e "\033[00;32mbuilding $(LIBRARY)\033[00m"
	@echo "var rs $(LIBRARY) $(OBJ)"
	@ar vrs $(LIBRARY) $(OBJ)\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"
	@echo ""

%.o	: %.c $(COMM_DEP)
	@echo -e "\033[00;32mbuilding $@\033[00m"
	@echo "$(CC) -c $(INC_FLAG) $(CFLAGS) $(OPT) $< -o $@"
	@$(CC) -c $(INC_FLAG) $(CFLAGS) $(OPT) $< -o $@\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"
	@echo ""

clean	:
	@echo -e "\033[00;32mcleaning $(OBJ)\033[00m"
	@echo "rm -f $(OBJ)"
	@rm -f $(OBJ)\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"
	@echo ""

	@echo -e "\033[00;32mcleaning $(LIBRARY)\033[00m"
	@echo "rm -f $(LIBRARY)"
	@rm -f $(LIBRARY)\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"
	@echo ""

#installation rules
install	: $(LIBRARY)
	@echo -e "\033[00;32minstaling $(LIBRARY)\033[00m"
	@echo "skipped"
	@echo ""

uninstall:
	@echo -e "\033[00;32muninstalling $(LIBRARY)\033[00m"
	@echo "skipped"
	@echo ""

