CC = g++
CFLAGS = -Wall -std=c++11
SRC_DIR = src
BUILD_DIR = build
TARGET = build/sjf

# If FILE argument is not provided, use a default value
FILE ?= ./test_cases/process1.dat

$(shell mkdir $(BUILD_DIR))

all: build run

build: src/Scheduler.cpp src/Main.cpp src/Scheduler.h src/Process.h
	$(CC) $(CFLAGS) -c src/Scheduler.cpp -o build/scheduler.o
	$(CC) $(CFLAGS) -c src/Main.cpp -o build/main.o
	$(CC) $(CFLAGS) build/scheduler.o build/main.o -o $(TARGET)

run:
	./$(TARGET) $(FILE)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean run build
