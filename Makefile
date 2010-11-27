DIRS	= lib main

#default rule
default	:
	@for i in $(DIRS); do ( cd $$i; $(MAKE) | sed -e "/^[ 	]*[^ 	]/s/^/"`echo -e "\033[00;32m$$i:\033[00m" | sed -e "s/\//\\\\\\\\\\\//"`"	/" ) | sed -e "1 d; $$ d"; done\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"

.DEFAULT:
	@for i in $(DIRS); do ( cd $$i; $(MAKE) $@ | sed -e "/^[ 	]*[^ 	]/s/^/"`echo -e "\033[00;32m$$i:\033[00m" | sed -e "s/\//\\\\\\\\\\\//"`"	/" ) | sed -e "1 d; $$ d"; done\
		2>&1 3>&2 1>&3| sed -e "s/^/"`echo -e "\033[00;31m"`"/" -e "s/$$/"`echo -e "\033[00m"`"/"
