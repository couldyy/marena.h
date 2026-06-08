CC=gcc

FLAGS=-Wall -Wextra -g -O0
BUILD_DIR=./build
TESTS_DIR=./tests
all: test_performance test

tests: test_performance test zeroed

make_build_dir:
	mkdir -p ./build

test_performance: make_build_dir marena.h $(TESTS_DIR)/test_performance.c
	$(CC) $(FLAGS) -lm -o $(BUILD_DIR)/test_performance $(TESTS_DIR)/test_performance.c

test: make_build_dir marena.h $(TESTS_DIR)/test.c
	$(CC) $(FLAGS) -o $(BUILD_DIR)/test $(TESTS_DIR)/test.c

zeroed: make_build_dir marena.h $(TESTS_DIR)/zeroed.c
	$(CC) $(FLAGS) -o $(BUILD_DIR)/zeroed $(TESTS_DIR)/zeroed.c
