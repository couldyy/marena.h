CC=gcc

FLAGS=-Wall -Wextra -g -O0
BUILD_DIR=./build
TESTS_DIR=./tests
EXAMPLES_DIR=./examples

all: test_performance tests examples

tests: test_performance test zeroed

examples: make_build_dir usage

usage: make_build_dir $(EXAMPLES_DIR)/usage.c
	$(CC) $(FLAGS) -o $(BUILD_DIR)/usage $(EXAMPLES_DIR)/usage.c

make_build_dir:
	mkdir -p ./build

# main program for running tests
tester: make_build_dir
	$(CC) $(FLAGS) -o $(BUILD_DIR)/tester $(TESTS_DIR)/tester.c

test_performance: make_build_dir marena.h $(TESTS_DIR)/test_performance.c
	$(CC) $(FLAGS) -lm -o $(BUILD_DIR)/test_performance $(TESTS_DIR)/test_performance.c

test: make_build_dir marena.h $(TESTS_DIR)/test.c
	$(CC) $(FLAGS) -o $(BUILD_DIR)/test $(TESTS_DIR)/test.c

zeroed: make_build_dir marena.h $(TESTS_DIR)/zeroed.c
	$(CC) $(FLAGS) -o $(BUILD_DIR)/zeroed $(TESTS_DIR)/zeroed.c
