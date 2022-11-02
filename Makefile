CC=g++
CFLAGS= -g -fsanitize=address -std=c++20 -Wall -Wextra -Werror -pedantic
OBJS = sha256.cc

TARGET=sha256

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RN) $(TARGET) $(OBJS)

.PHONY: clean
