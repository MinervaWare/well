CC= gcc
CFLAGS= -g -O2 -std=c89 -Iinclude -Wall -Wframe-larger-than=2048  -Wno-implicit-function-declaration -Wno-unused-variable -Wno-unused-result 
CFLAGS_REL= -O2 -std=c89 -Iinclude -fomit-frame-pointer -fconserve-stack -Wall -Wframe-larger-than=2048  -Wno-implicit-function-declaration -Wno-unused-variable -Wno-unused-result

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

release: $(OBJS)
	$(CC) $(CFLAGS_REL) $(OBJS) -o $(BIN)

run_test:
	well tests/helloWorld/helloworld.well -i

cvim:
	mkdir -p ~/.vim/syntax
	mkdir -p ~/.vim/ftdetect
	cp vim/syntax/well.vim ~/.vim/syntax/
	cp vim/ftdetect/well.vim ~/.vim/ftdetect/

clean_vim:
	rm -f ~/.vim/syntax/well.vim
	rm -f ~/.vim/ftdetect/well.vim

clean:
	rm $(OBJS) $(BIN)
