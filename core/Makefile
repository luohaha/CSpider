TARGET=libcspider.dylib
CC=gcc
CFLAGS=-ggdb -o0 -W -Wall -I includes -fpic
LIBFLAG=-luv -lcurl -lpcre
C_SOURCES=$(shell find . -name "*.c")
C_OBJECTS=$(patsubst %.c, %.o, $(C_SOURCES))
HEADERS=$(shell find . -name "*.h")
TEMP=$(shell find . -name "*~")

$(TARGET): $(C_OBJECTS)
	$(CC) -shared -o $(TARGET) $(C_OBJECTS) $(LIBFLAG)
move:
	sudo cp $(TARGET) /usr/lib
	if [ ! -d "/usr/include/cspider" ]; then sudo mkdir /usr/include/cspider; fi;
	sudo cp $(HEADERS) /usr/include/cspider

%.o: $(C_SOURCES) %.h
	$(CC) -c $(CFLAGS) $(LIBFLAG)$< -o $@
clean:
	rm -f $(shell find . -name "*.o") $(TEMP) $(TARGET)
