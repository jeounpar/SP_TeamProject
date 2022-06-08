#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <pthread.h>
#include "../include/ft_gpio.h"
#include "../include/ft_pwm.h"
#include "../include/ft_thread.h"
#include "../include/ft_socket.h"

void error_handling(char *message);
int right_client_main();
int left_client_main();

static char* getUUID(){
    // length of raspberry device uuid.
    char *uuid = (char*)malloc(32 * sizeof(char)); 
    
    FILE* uuid_file = fopen("/etc/machine-id", "r");
    fgets(uuid, 32, uuid_file);

    // printf("Device ID : %s\n", uuid);

    fclose(uuid_file);

    return uuid;
}

int main(void) {

	// 1번 라즈베리파이
    if (strcmp(getUUID(), "401d9479da094aa6a86b4e18d9bf676") == 0){
		int serv_sock;
		client_t *client_data;
		serv_sock = server_sock_open();
		client_data = (client_t *)malloc(2 * sizeof(client_t));
		client_sock_accept(client_data, serv_sock);
		ft_pthread_init(client_data, serv_sock);
		close_sock(client_data, serv_sock);
    }

    // 2번 라즈베리파이
    else if (strcmp(getUUID(), "316ae05cfd87495fa67a75f8ec53365") == 0){
        left_client_main();
    }

    // 3번 라즈베리파이
    else if (strcmp(getUUID(), "9713a22a4fcc4ae58e1fe2f9f301c82") == 0){
        right_client_main();
    }

	return(0);
}
