#VARIABLES
CC = gcc
CFLAGS = -Wall
EXEC = testUSB.exe
DEPENDENCIES = testUSB.c
LINKS = usb-1.0


$(EXEC) : $(DEPENDENCIES)
	$(CC) -o $@ $^ $(CFLAGS) -l $(LINKS)

test :
	./$(EXEC)

clean :
	rm $(EXEC)
