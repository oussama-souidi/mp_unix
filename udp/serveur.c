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


int fd = socket(AF_INET, SOCK_DGRAM, 0);
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
while(1) {
len = sizeof(client);

int n = recvfrom(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &len);
if (n < 0) {
    perror("échec du réception du message");
    exit(0);    
}
/*buffer[n] = '\0';
puts(buffer);

uint32_t x = atoi(buffer);
srand(time(NULL));

for (int i = 0; i < x; i++) {
    int randomNumber = rand();
    list[i] = randomNumber;
}
sendto(fd, &list, sizeof(list), 0, (struct sockaddr *)&client, len);
*/

printf("Nombre reçu du client: %d\n", *(int *)buffer);
int x = *(int *)buffer;
srand(time(NULL));
//x = rand() % 100 + 1;
//sendto(fd, &x, sizeof(x), 0, (struct sockaddr *)&client, len);
int list[NMAX];
for (int i = 0; i < x; i++) {
    int randomNumber = rand()% 1000 + 1;
    list[i] = randomNumber;
}
sendto(fd, &list, sizeof(list), 0, (struct sockaddr *)&client, len);
}}