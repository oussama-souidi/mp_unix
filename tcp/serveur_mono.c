#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "services.h"
#include <unistd.h>

#define PORT 5000
#define NMAX 100
#define MAXLINE 1000
int main()
{
    char buffer[NMAX];
    int max_connexions = 1;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("échec de création de socket");
        return -1;
    }

    struct sockaddr_in addresse, client;
    {
        addresse.sin_family = AF_INET;
        addresse.sin_addr.s_addr = htonl(INADDR_ANY);
        addresse.sin_port = htons(PORT);
    };
    if (bind(fd, (struct sockaddr *)&addresse, sizeof(addresse)) < 0)
    {
        perror("échec du bind");
        exit(0);
    }

    listen(fd, max_connexions);
    socklen_t client_len = sizeof(client);
    int client_sock = accept(fd, (struct sockaddr *)&client, &client_len);
    if (client_sock < 0)
    {
        perror("échec de création de socket");
        return -1;
    }
    printf("Client connecté\n");
    
    int n = read(client_sock, buffer, sizeof(buffer) - 1);
    buffer[n] = 0;
    
    char user[50], password[50];
    sscanf(buffer, "LOGIN %49s %49s", user, password);
    
    if(!check_login(user, password))
    {
        char *msg = "FAIL\n";
        write(client_sock, msg, strlen(msg));
        close(client_sock);
        close(fd);
        return 0;
    }
    write(client_sock, "OK\n", 3);
    time_t start = time(NULL);
    
    while (1)
    {
        n = read(client_sock, buffer, sizeof(buffer) - 1);
        if (n <= 0)
        {
            perror("échec de la réception");
            break;
        }
        buffer[n] = '\0';
        printf("Message reçu: %s\n", buffer);
        if (strncmp(buffer, "LIST", 4)==0) {
            send_services_list(client_sock);
        } else if (strncmp(buffer, "SERVICE", 7)==0) {
            int code; sscanf(buffer, "SERVICE %d", &code);
            execute_service(client_sock, code, start);
            if (code==5) break;
        } else {
            write(client_sock, "Commande inconnue\n", 18);
            printf("Commande inconnue: %s\n", buffer);
        }
    }

    return 0;
}