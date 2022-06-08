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

int right_client_main()
{
    int server_sock;
    struct sockaddr_in serv_addr;
    char msg[5];

    // if (export_sensor_gpio() == 1 || direction_sensor_gpio() == 1)
    //     return 1;

    server_sock = server_sock_connect();

    while (1) {
        // int sensor4 = GPIORead(25) * 16;
        // int sensor5 = GPIORead(23) * 32;
        // int sensor6 = GPIORead(6) * 64;
        // int sum = sensor4 + sensor5 + sensor6;
        int sum = 123;
        sprintf(msg, "%d", sum);
        // printf("4 : %d, 5 : %d, 6: %d\n", sensor4, sensor5, sensor6);
        write(server_sock, msg, sizeof(msg));
        usleep(100000);
    }

    close(server_sock);
    //Disable GPIO pins
    if (unexport_sensor_gpio() == 4)
        return(4);
    return(0);
}

int left_client_main()
{
    int server_sock;
	struct sockaddr_in serv_addr;
	char msg[5];

    // if (export_sensor_gpio() == 1 || direction_sensor_gpio() == 1)
    //     return 1;

    server_sock = server_sock_connect();
    
	while (1) {
        // int sensor3 = GPIORead(25) * 8;
        // int sensor2 = GPIORead(23) * 4;
        // int sensor1 = GPIORead(6) * 2;  
        // int sum = sensor1 + sensor2 + sensor3;
        int sum = 456;
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
