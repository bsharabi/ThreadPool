.PHONY: all
CC = gcc
CXXFLAG = -Wall -g


all: task stdinExample thread


thread:	codec.h threadPool.c threadPool.h
	$(CC) $(CXXFLAG)   threadPool.c ./libCodec.so -o coder -pthread


task: codec.h basic_main.c
	$(CC) $(CXXFLAG) basic_main.c ./libCodec.so -o encoder

stdinExample: stdin_main.c
	$(CC) $(CXXFLAG) stdin_main.c ./libCodec.so -o tester


.PHONY: clean
clean:
	-rm coder encoder tester 2>/dev/null
