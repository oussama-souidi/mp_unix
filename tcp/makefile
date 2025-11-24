# Makefile

CC = gcc
CFLAGS = -Wall

all: serveur_mono serveur_multi client_mono client_multi

serveur_mono: serveur_mono.c services.c
	$(CC) $(CFLAGS) serveur_mono.c services.c -o serveur_mono

serveur_multi: serveur_multi.c services.c
	$(CC) $(CFLAGS) serveur_multi.c services.c -o serveur_multi


client_mono: client_mono.c
	$(CC) $(CFLAGS) client_mono.c -o client_mono

client_multi: client_mono.c
	$(CC) $(CFLAGS) client_multi.c -o client_multi


clean:
	rm -f serveur_mono serveur_multi client_mono client_multi