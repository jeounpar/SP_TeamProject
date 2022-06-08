#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/ft_thread.h"

void error_handling(char *message);

int server_sock_open() {

	int serv_sock;
	struct sockaddr_in serv_addr;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	memset(&serv_addr, 0 , sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi("8080"));
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock,5) == -1)
		error_handling("listen() error");
	return serv_sock;
}

int server_sock_connect() {
	struct sockaddr_in serv_addr;

	int server_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (server_sock == -1)
        error_handling("socket() error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("172.30.1.10");
    serv_addr.sin_port = htons(atoi("8080"));
    if(connect(server_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("connect() error");
	return server_sock;
}

void close_sock(client_t *client_data, int serv_sock)
{
	close(client_data[0].fd);
	close(client_data[1].fd);
	close(serv_sock);
	free(client_data);
}

void client_sock_accept(client_t *client_data, int serv_sock)
{
	int clnt_sock_fd;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;

	clnt_addr_size = sizeof(clnt_addr);
	for (int i = 0; i < 2; i++) {
		clnt_sock_fd = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_sock_fd == -1) error_handling("accept() error");
		client_data[i].fd = clnt_sock_fd;
		printf("%s\n", inet_ntoa(clnt_addr.sin_addr));
		strcpy(client_data[i].ip, inet_ntoa(clnt_addr.sin_addr));
	}
}