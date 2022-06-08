#ifndef FT_THREAD_H
# define FT_THREAD_H

#include "../include/ft_socket.h"

void ft_pthread_init(client_t *client_data, int serv_sock);
void close_sock(client_t *client_data, int serv_sock);

#endif