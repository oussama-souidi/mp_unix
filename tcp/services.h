#ifndef SERVICES_H
#define SERVICES_H

#include <time.h>

int check_login(const char* user, const char* pass);
void send_services_list(int fd);
void execute_service(int fd, int code, time_t start);

#endif
