CC=gcc
CFLAGS=-ggdb
LIBS=-lreadline -lm -lfl
SRC=src
OBJECTS= main.o repl.o ast.o environment.o 
GRAM= lex.yy.c parser.tab.c parser.tab.h
TARGET=bim

all: $(TARGET)

.PHONY: clean

$(TARGET): $(GRAM) $(OBJECTS)
	$(CC) $(OBJECTS) $(GRAM) $(LIBS) -o $(TARGET)
	
%.o : %.c
	$(CC) $(CFLAGS) -c $<
	
parser.tab.c parser.tab.h: parser.y ast.h
	bison -d parser.y
	
lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

clean:
	rm -f *.o $(GRAM)
	rm -f $(TARGET)
