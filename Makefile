# Declaratiile de variabile
CC = gcc
CFLAGS = -g -Wall  -lncurses
 

battleship: meniu3.c nou.c
	$(CC) -o battleship meniu3.c nou.c nyan.c flames.c $(CFLAGS)
build:
	$(CC) -o battleship meniu3.c nou.c nyan.c flames.c $(CFLAGS)
run:
	./battleship configuratie.txt configuratie2.txt

clean:
	rm ./battleship
