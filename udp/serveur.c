#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define NMAX 100
#define BUFFER_SIZE 1000

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    char buffer[BUFFER_SIZE];

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("échec de création de socket");
        return -1;
    }

    struct sockaddr_in serveur, client;
    {
        serveur.sin_family = AF_INET;
        serveur.sin_addr.s_addr = INADDR_ANY;
        serveur.sin_port = htons(port);
    };

    if (bind(fd, (struct sockaddr *)&serveur, sizeof(serveur)) < 0)
    {
        perror("échec du bind");
        return -1;
    }
    printf("Serveur démarré sur le port %d\n", port);

    while (1)
    {
        int len = sizeof(client);

        if (recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &len) < 0)
        {
            perror("échec du réception du message");
            return -1;
        }
        printf("Client connecté: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        printf("Nombre reçu du client: %d\n", *(int *)buffer);

        int x = *(int *)buffer;
        srand(time(NULL));

        int list[NMAX];
        for (int i = 0; i < x; i++)
        {
            int randomNumber = rand() % 1000 + 1;
            list[i] = randomNumber;
        }

        printf("Les %d nombres envoyés au client: ", x);
        for (int i = 0; i < x; i++)
        {
            printf("%d ", list[i]);
        }
        printf("\n");
        
        sendto(fd, &list, sizeof(list), 0, (struct sockaddr *)&client, len);
    }
    return 0;
}