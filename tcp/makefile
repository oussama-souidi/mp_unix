CC=gcc
CFLAGS=-Wall

all: serveur_mono serveur_multi serveur_generic client_mono client_multi client_ms

serveur_mono: serveur_mono.c services.c
	$(CC) $(CFLAGS) serveur_mono.c services.c -o serveur_mono

serveur_multi: serveur_multiclient.c services.c
	$(CC) $(CFLAGS) serveur_multiclient.c services.c -o serveur_multi

serveur_generic: serveur_generic.c services.c
	$(CC) $(CFLAGS) serveur_generic.c services.c -o serveur_generic

client_mono: client_mono.c
	$(CC) $(CFLAGS) client_mono.c -o client_mono

client_multi: client_mono.c
	$(CC) $(CFLAGS) client_mono.c -o client_multi

client_ms: client_multi_server.c
	$(CC) $(CFLAGS) client_multi_server.c -o client_ms

clean:
	rm -f serveur_mono serveur_multi serveur_generic client_mono client_multi client_ms
