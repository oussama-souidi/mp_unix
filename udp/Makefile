# Makefile

CC = gcc
CFLAGS = -Wall

all: client serveur

client: client.c
	$(CC) $(CFLAGS) client.c -o client

serveur: serveur.c
	$(CC) $(CFLAGS) serveur.c -o serveur

clean:
	rm -f client serveur



	