
INCLUDE_DIR = -I.

SOURCES = main.c lib.c 

OBJECTS := $(notdir $(SOURCES:.c=.o))

all : $(OBJECTS)
	$(CC) $(OBJECTS) -o sensor 

%.o : %.c
	$(CC) -g $(CFLAGS) $(INCLUDE_DIR) -o $@ -c $<
clean:
	rm -rf *.o
