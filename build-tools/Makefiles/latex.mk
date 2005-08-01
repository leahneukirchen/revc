# latex.mk: build documentation from LaTeX sources
#
################################################################
# Copyright (C) 2001 Tom Lord
# Copyright (C) 2003 Stig Brautaset
# 
# See the file "COPYING" for further information about
# the copyright and warranty status of this work.
# 



ifndef latex-mk
latex-mk		:= 1

# Build documentation from LaTeX sources
#
# Optionally set on entry:
#
#	$(latex-docs)	LaTeX file to pass to `latex' 
#			(default: "manual.tex")
#
#


include $(makefiles)/rules.mk

ifndef latex-docs
latex-docs		:=	manual.tex
endif

latex-files		:=	$(wildcard *.tex)

latex-build-dir		:=	latex-tmp-dir



link-latex-files: $(latex-files)
	mkdir -p $(latex-build-dir)
	cd $(latex-build-dir) ; \
	for f in * ,,fake.tex ; do \
	  test ! -e $$f || rm -r $$f ; \
	done ; \
	ln -s $(srcdir)/* . 


clean: clean-latex-build-files

clean-doc: clean-latex-build-files

clean-latex-build-files:
	for f in $(latex-build-dir) ; do \
	  test ! -e $$f || rm -r $$f ; \
	done

endif

# arch-tag: Stig Brautaset Sat Mar  8 23:19:03 GMT 2003 (latex.mk)
#
