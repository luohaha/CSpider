CC=gcc
RM=rm -f
CFLAGS=-ggdb -O0 -W -Wall
LIBFLAG=-luv -lcurl
TARGET=download

C_SOURCES=download.c

OBJECTS=$(C_SOURCES:.c=.o)

lhttp: $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBFLAG)

%.o: %.c
	$(CC) -c $(CFLAGS) $(LIBFLAG) $< -o $@

clean:
	rm -f $(OBJECTS) *.*~ $(TARGET) *.download
