CC=gcc

FLAGS=-Wall -Wextra -g
BUILD_DIR=./build
all: test_performance test

make_build_dir:
	mkdir -p ./build

test_performance: make_build_dir marena.h test_performance.c
	$(CC) $(FLAGS) -o $(BUILD_DIR)/test_performance test_performance.c

test: make_build_dir marena.h test.c
	$(CC) $(FLAGS) -o $(BUILD_DIR)/test test.c
