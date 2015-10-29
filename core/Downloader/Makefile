CC=gcc
RM=rm -f
CFLAGS=-ggdb -O3 -W -Wall
LIBFLAG=-luv

C_SOURCES=test.c \
          lhttp.c \
          http_res_pro.c \

OBJECTS=$(C_SOURCES:.c=.o)

lhttp: $(OBJECTS)
	$(CC) -o lhttp $(OBJECTS) $(LIBFLAG)

%.o: %.c
	$(CC) -c $(CFLAGS) $(LIBFLAG) $< -o $@

clean:
	rm -f $(OBJECTS) *.*~ lhttp

