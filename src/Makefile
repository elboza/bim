CC=gcc
LIBFLEX=-lfl
ifeq ($(FLEX),osx)
LIBFLEX=-ll
endif
CFLAGS=-ggdb -Wall -std=gnu99
LIBS=-lreadline -lm $(LIBFLEX)
SRC=src
OBJECTS= main.o repl.o ast.o env.o eval.o print.o 
GRAM= lex.yy.c parser.tab.c
RMGRAM= $(GRAM) parser.tab.h parser.output
TARGET=bim

all: $(TARGET)

.PHONY: clean

$(TARGET): $(GRAM) $(OBJECTS)
	$(CC) $(OBJECTS) $(GRAM) $(LIBS) -o $(TARGET)
	
%.o : %.c
	$(CC) $(CFLAGS) -c $<
	
parser.tab.c parser.tab.h: parser.y ast.h
	bison -d -v parser.y
	
lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

clean:
	rm -f *.o $(RMGRAM)
	rm -f $(TARGET)
