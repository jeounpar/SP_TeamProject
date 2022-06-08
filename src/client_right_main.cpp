#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include "../include/ft_gpio.h"
#include "../include/ft_socket.h"

void error_handling(char *message);

int main(void)
{
    int server_sock;
	struct sockaddr_in serv_addr;
	char msg[5];

    if (export_sensor_gpio() == 1 || direction_sensor_gpio() == 1)
        return 1;

    server_sock = server_sock_connect();
    
	while (1) {
        int sensor3 = GPIORead(25) * 8;
        int sensor2 = GPIORead(23) * 4;
        int sensor1 = GPIORead(6) * 2;  
        int sum = sensor1 + sensor2 + sensor3;
        sprintf(msg, "%d", sum);
        // printf("1 : %d, 2 : %d, 3: %d\n", sensor1, sensor2, sensor3);
        write(server_sock, msg, sizeof(msg));
        usleep(100000);
    }

	close(server_sock);
	//Disable GPIO pins
    if (unexport_sensor_gpio() == 4)
        return(4);
	return(0);
}