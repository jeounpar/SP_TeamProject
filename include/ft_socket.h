#ifndef FT_SOCKET_H
# define FT_SOCKET_H

typedef struct client_s{
	int fd;
	char ip[30];
} client_t;

int server_sock_open();
int server_sock_connect();
void client_sock_accept(client_t *client_data, int serv_sock);
void close_sock(client_t *client_data, int serv_sock);

#endif