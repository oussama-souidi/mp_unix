# Makefile

CC = gcc
CFLAGS = -Wall

all: serveur_mono serveur_multi client

serveur_mono: serveur_mono.c services.c
	$(CC) $(CFLAGS) serveur_mono.c services.c -o serveur

serveur_multi: serveur_multi.c services.c
	$(CC) $(CFLAGS) serveur_multi.c services.c -o serveur


client: client.c
	$(CC) $(CFLAGS) client.c -o client


clean:
	rm -f serveur_mono serveur_multi client
