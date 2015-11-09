CC=gcc
RM=rm -f
CFLAGS=-ggdb -O0 -W -Wall
LIBFLAG=-luv -lhttp_parser

C_SOURCES=test.c \
          request.c \
          http_res_parser.c \

OBJECTS=$(C_SOURCES:.c=.o)

lhttp: $(OBJECTS)
	$(CC) -o lhttp $(OBJECTS) $(LIBFLAG)

%.o: %.c
	$(CC) -c $(CFLAGS) $(LIBFLAG) $< -o $@

clean:
	rm -f $(OBJECTS) *.*~ lhttp

