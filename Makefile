CC=gcc

FLAGS=-Wall -Wextra -g
all: test_performance test

test_performance: marena.h test_performance.c
	$(CC) $(FLAGS) -o test_performance test_performance.c

test: marena.h test.c
	$(CC) $(FLAGS) -o test test.c
