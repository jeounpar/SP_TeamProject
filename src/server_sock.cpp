#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

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
    serv_addr.sin_addr.s_addr = inet_addr("192.168.1.2");
    serv_addr.sin_port = htons(atoi("8080"));
    if(connect(server_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("connect() error");
	return server_sock;
}