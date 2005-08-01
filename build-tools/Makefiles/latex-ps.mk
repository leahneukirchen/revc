# latex-ps.mk: build postscript documentation from LaTeX sources
#
################################################################
# Copyright (C) 2001 Tom Lord
# Copyright (C) 2003 Stig Brautaset
# 
# See the file "COPYING" for further information about
# the copyright and warranty status of this work.
# 



ifndef latex-ps-mk
latex-ps-mk		:= 1

# Build ps documentation from LaTeX sources
#
# Set on entry:
#
#	$(latex-docs)	LaTeX file to pass to `latex' 
#			(default: "manual.tex")
#
#
# After "make ps", the build directory will contain .ps file(s) of
# documentation compiled from LaTeX sources.
#


include $(makefiles)/latex.mk

ps-files		:=	$(patsubst %.tex,%.ps,$(latex-docs))

$(ps-files): link-latex-files
	cd $(latex-build-dir) ; \
	latex $(patsubst %.ps,%.tex,$@) ; \
	latex $(patsubst %.ps,%.tex,$@) ; \
	dvips -o $@ $(patsubst %.ps,%.dvi,$@) ; \
	mv $@ ..

ps PS: $(ps-files)
	echo $(ps-files) > PS

doc: ps

clean: clean-ps-files

clean-doc: clean-ps-files 

clean-ps-files:
	for f in $(ps-files) PS ; do \
	  test ! -e $$f || rm -r $$f ; \
	done

endif

# arch-tag: Stig Brautaset Sat Mar  8 23:19:03 GMT 2003 (latex-ps.mk)
#
