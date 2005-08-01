# install-datafiles.mk: install datafiles
#
################################################################
# Copyright (C) 2001, 2002 Tom Lord
# Copyright (C) 2003 Stig Brautaset
# 
# See the file "COPYING" for further information about
# the copyright and warranty status of this work.
# 



ifndef install-datafiles-mk
install-datafiles-mk	:= 1

# Set on entry:
#
#	$(datafiles)	the datafiles that should be installed
#
# After installation, the build directory will contain
# "Installed-datafiles" which is a list of the files that were
# installed.
#

include $(makefiles)/rules.mk

install: install-datafiles Installed-datafiles

install-datafiles Installed-datafiles: $(datafiles)
ifdef datafiles
	for f in $(datafiles) ; do test ! -e $(data-install-dir)/$$f || rm -f $(data-install-dir)/$$f ; done
	mkdir -p $(data-install-dir)
	for f in $(datafiles) ; do cp $(srcdir)/$$f $(data-install-dir)/$$f ; done
endif
	echo $(datafiles) | sed -f $(makefiles)/column.sed > Installed-datafiles

clean: clean-install-datafiles

clean-install-datafiles:
	test ! -e Installed-datafiles || rm Installed-datafiles

endif

# arch-tag: Stig Brautaset Sun Mar  9 17:02:49 GMT 2003 (install-datafiles.mk)
#
