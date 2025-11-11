CC= gcc
CFLAGS= -g -O2 -std=c89 -Iinclude

SRCS:= $(wildcard src/*.c) \
	   $(wildcard include/*.c)
OBJS:= $(SRCS:.c=.o)

BIN= well

.PHONY: all base run_test vim clean_vim clean

all: base

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

base: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)

run_test:
	well tests/helloWorld/helloworld.well -i

vim:
	mkdir -p ~/.vim/syntax
	mkdir -p ~/.vim/ftdetect
	cp vim/syntax/well.vim ~/.vim/syntax/
	cp vim/ftdetect/well.vim ~/.vim/ftdetect/

clean_vim:
	rm -f ~/.vim/syntax/well.vim
	rm -f ~/.vim/ftdetect/well.vim

clean:
	rm $(OBJS) $(BIN)
