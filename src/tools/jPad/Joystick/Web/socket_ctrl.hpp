#pragma once

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "types.h"

int init_server(uint16_t port);
int waitConnection(int srv_fd);
int send_all(int fd, cStrRO buf, size_t len) ;
