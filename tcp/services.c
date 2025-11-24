#include "services.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>

int check_login(const char *user, const char *password)
{
    return (strcmp(user, "admin") == 0 && strcmp(password, "1234") == 0) || (strcmp(user, "user") == 0 && strcmp(password, "pass") == 0);
}

void send_services_list(int fd)
{
    char msg[] =
        "1) Date et heure\n"
        "2) Liste fichiers (répertoire courant)\n"
        "3) Envoi contenu d'un fichier (nom demandé après)\n"
        "4) Durée depuis connexion\n"
        "5) Quitter\n";
    write(fd, msg, strlen(msg));
}

void execute_service(int fd, int code, time_t start)
{
    char buffer[4096];
    if (code == 1)
    {
        time_t t = time(NULL);
        snprintf(buffer, sizeof(buffer), "Date: %s", ctime(&t));
        write(fd, buffer, strlen(buffer));
    }
    else if (code == 2)
    {
        DIR *d = opendir(".");
        if (!d)
        {
            write(fd, "Erreur opendir\n", 15);
            return;
        }
        struct dirent *ent;
        buffer[0] = 0;
        while ((ent = readdir(d)) != NULL)
        {
            strncat(buffer, ent->d_name, sizeof(buffer) - strlen(buffer) - 2);
            strncat(buffer, "\n", sizeof(buffer) - strlen(buffer) - 1);
        }
        closedir(d);
        write(fd, buffer, strlen(buffer));
    }
    else if (code == 3)
    {

        int n = read(fd, buffer, sizeof(buffer) - 1);
        if (n <= 0)
        {
            write(fd, "Aucun nom reçu\n", 14);
            return;
        }
        buffer[n] = 0;

        char *newline = strchr(buffer, '\n');
        if (newline)
            *newline = 0;
        FILE *f = fopen(buffer, "r");
        if (!f)
        {
            write(fd, "Impossible d'ouvrir le fichier\n", 31);
            return;
        }
        size_t r = fread(buffer, 1, sizeof(buffer) - 1, f);
        buffer[r] = 0;
        fclose(f);
        write(fd, buffer, strlen(buffer));
    }
    else if (code == 4)
    {
        long elapsed = time(NULL) - start;
        snprintf(buffer, sizeof(buffer), "Durée: %ld secondes\n", elapsed);
        write(fd, buffer, strlen(buffer));
    }
    else if (code == 5)
    {

        write(fd, "Bye\n", 4);
    }
    else
    {

        write(fd, "Code inconnu\n", 13);
    }
}
