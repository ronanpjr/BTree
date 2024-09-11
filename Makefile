# Compiler
CC = gcc

# Compiler flags
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wformat=2 -fsanitize=address,undefined,leak -g -Og
# Source files
SRCS = $(wildcard *.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = btree

# Default target
all: $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)

