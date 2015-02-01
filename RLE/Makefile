SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)
CFLAGS= -std=c99 -Wall -pedantic -g
EXECUTABLE=rle

$(EXECUTABLE): $(OBJECTS)
	gcc -o $(EXECUTABLE) $(OBJECTS)

%.o : %.c
	gcc -c $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
