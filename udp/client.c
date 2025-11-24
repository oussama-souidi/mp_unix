#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdbool.h>

#define NMAX 100
#define BUFFER_SIZE 1000
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <hostname> <port>\n", argv[0]);
        return 1;
    }

    char *hostname = argv[1];
    int port = atoi(argv[2]);

    char buffer[BUFFER_SIZE];
    struct sockaddr_in addresse;
    {
        addresse.sin_family = AF_INET;
        addresse.sin_addr.s_addr = inet_addr(hostname);
        addresse.sin_port = htons(port);
    };

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("échec de création de socket");
        return -1;
    }

    if (connect(fd, (struct sockaddr *)&addresse, sizeof(addresse)) < 0)
    {
        perror("échec de la connexion");
        exit(0);
    }
    printf("connexion établie avec le serveur \n");
    printf("Donner un entier entre 1 et 100: \n");

    int n;
    int res = scanf("%d", &n);
    bool valid = n >= 1 && n <= BUFFER_SIZE;

    while (!valid)
    {
        printf("Entrée invalide. Veuillez entrer un entier entre 1 et 100: \n");
        while (getchar() != '\n')
            ;
        int res = scanf("%d", &n);
        valid = n >= 1 && n <= BUFFER_SIZE;
    }

    send(fd, &n, sizeof(n), 0);
    recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
    printf("Les %d nombres reçus du serveur:\n", n);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", ((int *)buffer)[i]);
    }

    close(fd);
}