CC=g++
CFLAGS= -g -fsanitize=address -std=c++20 -Wall -Wextra -Werror -pedantic
OBJS = main.o sha256.o

TARGET=sha256

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(TARGET) $(OBJS)

.PHONY: clean
