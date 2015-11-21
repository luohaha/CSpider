TARGET=CSpider
CC=gcc
CFLAGS=-ggdv -o0 -W -Wall

C_SOURCE=main.c

OBJECTS=$(C_SOURCE:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) *.*~ $(TARGET)
