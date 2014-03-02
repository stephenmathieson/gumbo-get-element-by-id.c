
CC ?= gcc
SRC = $(wildcard src/*.c)
DEPS = $(wildcard deps/*/*.c)
OBJS = $(SRC:.c=.o)
OBJS += $(DEPS:.c=.o)
CFLAGS = -std=c99 -Ideps -Isrc
CFLAGS += -Wall -Wno-unused-function

test: $(OBJS) test.o
	$(CC) $^ -o $@ $(CFLAGS)
	./$@

%.o: %.c
	$(CC) $< -c -o $@ $(CFLAGS)

clean:
	rm -f test $(OBJS)

.PHONY: clean test
