# arch-tag: Tom Lord Tue Dec  4 14:47:18 2001 (scm-library.mk)
#
# scm-library.mk -
#
################################################################
# Copyright (C) 2001 Tom Lord
# 
# See the file "COPYING" for further information about
# the copyright and warranty status of this work.
# 



ifndef scm-library-mk
scm-library-mk	:= 1

# See also "library.mk"
#
# This file builds a library from source files that must be
# preprocessed to build ".x" files.  Such source files are
# designed to be compiled and linked with Systas Scheme.
#

include $(makefiles)/library.mk

xfiles		:=	$(patsubst %.c, %.x, $(libsource))

.SUFFIXES:	.x

$(xfiles):%.x:%.c %.h
	set -e ; \
	if test ! -e$(srcdir)/scmconfig.h ; then \
	  touch $(srcdir)/scmconfig.h; \
	  fake_scmconfig=1; \
	else \
	  fake_scmconfig=0; \
	fi; \
	touch $@; \
	rm -f ,$@; \
	$(CC) $(CFLAGS) -DSCM_MAGIC_SNARFER -E $< | sed -f $(makefiles)/dotx.sed > ,$@ ; \
	rm -f $@; \
	cp ,$@ $@ ; \
	$(CC) $(CFLAGS) -DSCM_MAGIC_SNARFER2 -E $< | sed -f $(makefiles)/dotx.sed >> ,$@ ; \
	rm -f $@; \
	mv ,$@ $@ ; \
	if test $$fake_scmconfig -eq 1 ; then \
	  test ! -e $(srcdir)/scmconfig.h || rm $(srcdir)/scmconfig.h; \
	fi

clean: clean-xfiles

clean-xfiles:
	for f in $(xfiles) ,fake.x ; do \
	  test ! -e $$f || rm -f $$f ; \
	done

$(libdepfiles): %.d: %.x

endif
