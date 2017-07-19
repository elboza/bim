.PHONY: clean

all: docs

docs:
	if [ -f /usr/bin/man2html ];	then man2html bim.man>bim.man.html ;fi
	
clean:
	rm -f  bin.man.html

