/* client_mono.c */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <hostname> <port>\n", argv[0]);
        return 1;
    }

    char *serveur = argv[1];
    int port = atoi(argv[2]);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addresse;
    {
        addresse.sin_family = AF_INET;
        addresse.sin_addr.s_addr = inet_addr(serveur);
        addresse.sin_port = htons(port);
    };
    if (connect(sock, (struct sockaddr *)&addresse, sizeof(addresse)) < 0)
    {
        perror("échec du connection");
        return -1;
    }
    char user[50], pass[50];
    printf("Utilisateur: ");
    scanf("%49s", user);
    printf("Mot de passe: ");
    scanf("%49s", pass);
    char buffer[4096];
    snprintf(buffer, sizeof(buffer), "LOGIN %s %s", user, pass);
    write(sock, buffer, strlen(buffer));
    int n = read(sock, buffer, sizeof(buffer) - 1);
    buffer[n] = 0;
    if (strcmp(buffer, "OK") != 0)
    {
        printf("Auth échouée: %s\n", buffer);
        return -1;
    }
    printf("Auth OK\n");
    while (1)
    {
        write(sock, "LIST\n", 5);
        n = read(sock, buffer, sizeof(buffer) - 1);
        buffer[n] = 0;
        printf("%s\n", buffer);
        int choix;
        printf("Choix: ");
        scanf("%d", &choix);
        getchar();
        snprintf(buffer, sizeof(buffer), "SERVICE %d\n", choix);
        write(sock, buffer, strlen(buffer));
        if (choix == 3)
        {
            // envoyer nom de fichier
            char fname[256];
            printf("Nom fichier: ");
            fgets(fname, sizeof(fname), stdin);
            write(sock, fname, strlen(fname));
        }
        n = read(sock, buffer, sizeof(buffer) - 1);
        if (n <= 0)
            break;
        buffer[n] = 0;
        printf("Résultat:\n%s\n", buffer);
        if (choix == 5)
            break;
    }
    close(sock);
    return 0;
}
