# latex-txt.mk: build plain text documentation from LaTeX sources
#
################################################################
# Copyright (C) 2001 Tom Lord
# Copyright (C) 2003 Stig Brautaset
# 
# See the file "COPYING" for further information about
# the copyright and warranty status of this work.
# 



ifndef latex-txt-mk
latex-txt-mk		:= 1

# Build plain text documentation from LaTeX sources
#
# Set on entry:
#
#	$(latex-docs)	LaTeX file to pass to `latex' 
#			(default: "manual.tex")
# 
# Optionally set on entry:
#
# 	$(html-to-text)	Program that takes a html-file 
# 			as its argument and produces a 
# 			plain text file on standard output
# 			(default: "links -dump")
#
# After "make txt", the build directory will contain .txt file(s) of
# documentation compiled from LaTeX sources.
#


include $(makefiles)/latex-html.mk

ifndef html-to-text
html-to-text		:= 	links -dump
endif

txt-files		:= 	$(patsubst %.tex,%.txt,$(latex-docs))

$(txt-files): $(html-files)
	cd $(latex-build-dir) ; \
	$(html-to-text) $(patsubst %.txt,%.html,$@) > $@ ; \
	mv $@ ..

txt TXT: $(txt-files)
	echo $(txt-files) > TXT

doc: txt

clean: clean-txt-files

clean-doc: clean-txt-files 

clean-txt-files:
	for f in $(txt-files) TXT ; do \
	  test ! -e $$f || rm -r $$f ; \
	done

endif

# arch-tag: Stig Brautaset Sun Mar  9 10:55:32 GMT 2003 (latex-txt.mk)
#
