# latex-html.mk: build html documentation from LaTeX sources
#
################################################################
# Copyright (C) 2001 Tom Lord
# Copyright (C) 2003 Stig Brautaset
# 
# See the file "COPYING" for further information about
# the copyright and warranty status of this work.
# 



ifndef latex-html-mk
latex-html-mk		:= 1

# Build html documentation from LaTeX sources
#
# Set on entry:
#
#	$(latex-docs)	LaTeX file to pass to `latex' 
#			(default: "manual.tex")
#
# Optionally set on entry:
#
# 	$(latex-to-html) 
# 			Program that creates a single 
# 			html-file from each LaTeX source file
# 			(default: "htlatex")
#
# After "make html", the build directory will contain .html file(s) of
# documentation compiled from LaTeX sources.
#


include $(makefiles)/latex.mk

html-files		:=	$(patsubst %.tex,%.html,$(latex-docs))

ifndef latex-to-html
latex-to-html		:= htlatex
endif

$(html-files): link-latex-files
	cd $(latex-build-dir) ; \
	$(latex-to-html) $(patsubst %.html,%.tex,$@) 

html HTML: $(html-files)
	cd $(latex-build-dir) ; \
	cp $(html-files) ..
	echo $(html-files) > HTML

doc: html

clean: clean-html-files

clean-doc: clean-html-files 

clean-html-files:
	for f in $(html-files) HTML ; do \
	  test ! -e $$f || rm -r $$f ; \
	done

endif

# arch-tag: Stig Brautaset Sun Mar  9 10:55:32 GMT 2003 (latex-html.mk)
#
