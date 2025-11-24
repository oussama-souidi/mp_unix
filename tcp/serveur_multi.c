/* serveur_multiclient.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include "services.h"

#define PORT 5001
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1000

void handle_client(int client)
{
    char buffer[BUFFER_SIZE];
    int n = read(client, buffer, sizeof(buffer) - 1);
    if (n <= 0)
    {
        close(client);
        exit(0);
    }
    buffer[n] = 0;
    char user[50], pass[50];
    sscanf(buffer, "LOGIN %49s %49s", user, pass);
    if (!check_login(user, pass))
    {
        write(client, "FAIL\n", 5);
        close(client);
        exit(0);
    }
    write(client, "OK\n", 3);
    time_t start = time(NULL);
    while (1)
    {
        n = read(client, buffer, sizeof(buffer) - 1);
        if (n <= 0)
            break;
        buffer[n] = 0;
        if (strncmp(buffer, "LIST", 4) == 0)
            send_services_list(client);
        else if (strncmp(buffer, "SERVICE", 7) == 0)
        {
            int code;
            sscanf(buffer, "SERVICE %d", &code);
            execute_service(client, code, start);
            if (code == 5)
                break;
        }
        else
            write(client, "Commande inconnue\n", 18);
    }
    close(client);
    exit(0);
}

int main()
{
    signal(SIGCHLD, SIG_IGN); // éviter zombies
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addresse;
    {
        addresse.sin_family = AF_INET;
        addresse.sin_port = htons(PORT);
        addresse.sin_addr.s_addr = INADDR_ANY;
        bind(fd, (struct sockaddr *)&addresse, sizeof(addresse));
    }
    listen(fd, MAX_CLIENTS);
    printf("Serveur multi-client écoute %d\n", PORT);
    while (1)
    {
        int client_sock = accept(fd, NULL, NULL);
        if (client_sock < 0)
            continue;
        pid_t pid = fork();
        if (pid == 0)
        {
            close(fd);
            handle_client(client_sock);
        } // fils
        else
        {
            close(client_sock);
        } // père continue
    }
    close(fd);
    return 0;
}
