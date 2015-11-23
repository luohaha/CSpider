TARGET=CSpider
CC=gcc
CFLAGS=-ggdb -o0 -W -Wall
LIBFLAG=-luv -lcurl
C_SOURCE=CSpider.c

OBJECTS=$(C_SOURCE:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LIBFLAG)

%.o: $(C_SOURCE) %.h
	$(CC) -c $(CFLAGS) $(LIBFLAG)$< -o $@

clean:
	rm -f $(OBJECTS) *~ $(TARGET)
