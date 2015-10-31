CC=gcc
RM=rm -f
CFLAGS=-ggdb -O3 -W -Wall
LIBFLAG=-luv -lhttp_parser

C_SOURCES=test.c \
          request.c \
          http_res_pro.c \

OBJECTS=$(C_SOURCES:.c=.o)

lhttp: $(OBJECTS)
	$(CC) -o lhttp $(OBJECTS) $(LIBFLAG)

%.o: %.c
	$(CC) -c $(CFLAGS) $(LIBFLAG) $< -o $@

clean:
	rm -f $(OBJECTS) *.*~ lhttp

