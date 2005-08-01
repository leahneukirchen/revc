# install-doc.mk: install documentation
#
################################################################
# Copyright (C) 2001, 2002 Tom Lord
# Copyright (C) 2003, Stig Brautaset
# 
# See the file "COPYING" for further information about
# the copyright and warranty status of this work.
# 



ifndef install-doc-mk
install-doc-mk		:= 1

# Optionally set on entry:
#
#	$(ps-files) 	postscript documentation
#	$(pdf-files) 	pdf documentation
#	$(html-files) 	html documentation
#	$(txt-files)	plain text documentation
# 	$(extradocs)	user-defined documentation 
# 			to be installed
# 			(default: README)
#
# After installation, the build directory will contain "Installed-docs"
# which is a list of the documentation files that were installed.
#

include $(makefiles)/rules.mk

documentation		:= $(ps-files) $(pdf-files) $(html-files) $(txt-files)
documentation		:= $(strip $(documentation))

ifndef documentation 
ifndef extradocs
extradocs 		:= README
endif
endif

install-doc: install-docs Installed-docs

install-docs Installed-docs: $(documentation)
	for f in $(documentation) .FAKE_DOC ; do test ! -e $(doc-install-dir)/$$f || rm -f $(doc-install-dir)/$$f ; done
	mkdir -p $(doc-install-dir)
	cp $(documentation) $(doc-install-dir)
	for f in $(extradocs) .FAKE_DOC ; do test ! -e $(srcdir)/$$f || cp $(srcdir)/$$f $(doc-install-dir) ; done
	echo $(documentation) $(extradocs) | sed -f $(makefiles)/column.sed > Installed-docs

clean: clean-install-docs

clean-install-docs:
	test ! -e Installed-docs || rm Installed-docs

endif

# arch-tag: Stig Brautaset Sun Mar  9 17:02:49 GMT 2003 (install-doc.mk)
#
