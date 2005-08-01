# latex-pdf.mk: build pdf documentation from LaTeX sources
#
################################################################
# Copyright (C) 2001 Tom Lord
# Copyright (C) 2003 Stig Brautaset
# 
# See the file "COPYING" for further information about
# the copyright and warranty status of this work.
# 



ifndef latex-pdf-mk
latex-pdf-mk		:= 1

# Build pdf documentation from LaTeX sources
#
# Set on entry:
#
#	$(latex-docs)	LaTeX file to pass to `latex' 
#			(default: "manual.tex")
#
#
# After "make pdf", the build directory will contain .pdf file(s) of
# documentation compiled from LaTeX sources.
#


include $(makefiles)/latex.mk

pdf-files		:=	$(patsubst %.tex,%.pdf,$(latex-docs))

$(pdf-files): link-latex-files
	cd $(latex-build-dir) ; \
	pdflatex $(patsubst %.pdf,%.tex,$@) ; \
	pdflatex $(patsubst %.pdf,%.tex,$@) ; \
	mv $@ ..

pdf PDF: $(pdf-files)
	echo $(pdf-files) > PDF

doc: pdf

clean: clean-pdf-files

clean-doc: clean-pdf-files 

clean-pdf-files:
	for f in $(pdf-files) PDF ; do \
	  test ! -e $$f || rm -r $$f ; \
	done

endif

# arch-tag: Stig Brautaset Sun Mar  9 10:25:41 GMT 2003 (latex-pdf.mk)
#
