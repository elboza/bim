CC=gcc
CFLAGS=-ggdb -Wall -std=gnu99
LIBS=-lreadline -lm -lfl
SRC=src
OBJECTS= main.o repl.o ast.o env.o 
GRAM= lex.yy.c parser.tab.c parser.tab.h
RMGRAM= $(GRAM) parser.output
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
