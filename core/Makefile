TARGET=CSpider
CC=gcc
CFLAGS=-ggdb -o0 -W -Wall -I includes
LIBFLAG=-luv -lcurl
C_SOURCES=$(shell find . -name "*.c")
C_OBJECTS=$(patsubst %.c, %.o, $(C_SOURCES))
TEMP=$(shell find . -name "*~")

$(TARGET): $(C_OBJECTS)
	$(CC) -o $(TARGET) $(C_OBJECTS) $(LIBFLAG)

%.o: $(C_SOURCES) %.h
	$(CC) -c $(CFLAGS) $(LIBFLAG)$< -o $@

clean:
	rm -f $(C_OBJECTS) $(TEMP) $(TARGET)
