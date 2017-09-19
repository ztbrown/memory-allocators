TARGET = test_suite
SRCS = test_suite.c $(wildcard ../src/*.c)
OBJS = $(SRCS:.c=.o)
LIB=../lib/libmock.a

CFLAGS=-std=c99 -Wall

all: $(TARGET)

%.o: %.c
		gcc $(CFLAGS) -c -o $@ $^

$(TARGET): $(OBJS) $(LIB)
		gcc $(CFLAGS) -o $@ $^ `pkg-config --cflags --libs check` `../bin/generate_mock_linker_flags.sh test_suite.c`

$(LIB):
		$(MAKE) -C ../lib

clean:
		rm -f $(TARGET) $(OBJS)
