#include <stdio.h>
#include <stdbool.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 5000 
#define NMAX 100
#define MAXLINE 1000
int main() {
char buffer[NMAX];
int len;

int fd = socket(AF_INET, SOCK_DGRAM, 0);
if (fd < 0) {
    perror("échec de création de socket");
    return -1;     
}  

struct sockaddr_in addresse;
{
    addresse.sin_family = AF_INET;
    addresse.sin_addr.s_addr = inet_addr("127.0.0.1");
    addresse.sin_port = htons(PORT);
};
if(connect(fd , (struct sockaddr *)&addresse, sizeof(addresse)) < 0) {
    perror("échec de la connexion");
    exit(0);
}
printf("connexion établie avec le serveur \n");
printf("Donner un entier entre 1 et 100: \n");

int n;
int res = scanf("%d", &n);
bool valid = n >= 1 && n <= NMAX;

while(!valid) {
    printf("Entrée invalide. Veuillez entrer un entier entre 1 et 100: \n");
    while(getchar() != '\n');
    int res = scanf("%d", &n);
    valid = n >= 1 && n <= NMAX;
}


send(fd, &n, sizeof(n), 0);
recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
int size = sizeof(&buffer);;
//printf("%d", size);
for(int i = 0; i < n; i++) {
    printf("%d\n", ((int *)buffer)[i]);
}

close(fd);
}