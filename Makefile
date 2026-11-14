
SRC_DIR = src
INC_DIR = include
CC = gcc
CFLAGS = -Wall -I$(INC_DIR) -Wextra

SRCS   = $(SRC_DIR)/banker.c $(SRC_DIR)/input.c $(SRC_DIR)/main.c
HEADER = $(INC_DIR)/banker.h
TARGET = banker


all: $(TARGET) 

$(TARGET): $(SRCS) $(HEADER)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

test: $(TARGET)
	./$(TARGET) 10 5 7
	diff -u result_esperado.txt result.txt

clean:
	rm -f $(TARGET) result.txt

.PHONY: all clean test