#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5000 
#define NMAX 100
#define MAXLINE 1000
int main() {
    char buffer[NMAX];
    int len;
    int list[NMAX];
    int max_connexions = 1;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
        perror("échec de création de socket");
        return -1;     
    }  

    struct sockaddr_in addresse, client;
    {
        addresse.sin_family = AF_INET;
        addresse.sin_addr.s_addr = INADDR_ANY;
        addresse.sin_port = htons(PORT);
    };
    if (bind(fd, (struct sockaddr *)&addresse, sizeof(addresse)) < 0) {
        perror("échec du bind");
        exit(0);    
    }
    listen(fd, max_connexions);
    int clientsock = accept(fd, (struct sockaddr *)&client, sizeof(client));

     while(1) {
        len = recv(clientsock, buffer, NMAX, 0);
        if (len <= 0) {
            perror("échec de la réception");
            break;
        }
        buffer[len] = '\0';
        printf("Message reçu: %s\n", buffer);

        // Echo back the received message
        send(clientsock, buffer, len, 0);

}