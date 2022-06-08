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
#include <opencv2/opencv.hpp>
#include "../include/ft_gpio.h"
#include "../include/ft_pwm.h"
#include "../include/ft_thread.h"
#include "../include/ft_socket.h"

void error_handling(char *message);

int g_client1_data = 0;
int g_client2_data = 0;

using namespace cv;
using namespace std;

void *client_thread_create(void *data) {
	int read_size;
	int msg_data = -1;
	int left_or_right = -1;
	char msg[5];
	client_t *client;

	client = (client_t *) data;
	printf("client ip = %s\n", client->ip);
	// 왼쪽
	if (strcmp(client->ip, "192.168.1.5") == 0)
		left_or_right = 0;
	// 오른쪽
	else if (strcmp(client->ip, "192.168.1.4") == 0)
		left_or_right = 1;
	while (1) {
		read_size = read(client->fd, msg, sizeof(msg));
		msg_data = atoi(msg);
		if (read_size <= 0)
			break;
		else {
			// left raspberry pi
			if (left_or_right == 0) {
				g_client1_data = msg_data;
			}
			// right raspberry pi
			else if (left_or_right == 1) {
				g_client2_data = msg_data;
			}
			printf("msg_data = %d\n", msg_data);
		}
	}
	close(client->fd);
}

void *piezo_control(void *data)
{
	Melody warning = Melody();
	warning.melody = {E, Dsharp, E, Dsharp, E, lowB, D, C,
		lowA, ZERO, ZERO, lowC, lowE, lowA, lowB, ZERO, lowE,
		lowG, lowB, C, ZERO, ZERO, E, Dsharp, E, Dsharp, E,
		lowB, D, C, lowA, ZERO, ZERO, lowC, lowE, lowA, lowB,
		ZERO, lowE, C, lowB, lowA, ZERO, ZERO};
	while (1) {
		if (g_client1_data > 0 || g_client2_data > 0) {
			warning.playMelody(0.3);
			usleep(1600000);
		}
		else
			PWMWriteDutyCycle(0, 0);
	}
}

void *camera_control(void *data) {
	VideoCapture cap(-1);
	if (!cap.isOpened()) {
		printf("camera open error\n");
		exit(1);
	}
	Mat img;
	printf("Camera on\n");
	while (1) {
		cap >> img;
		transpose(img, img);
        flip(img, img, 1);
		transpose(img, img);
        flip(img, img, 1);
		if (g_client1_data >= 0 && g_client2_data >= 0) {
			if (g_client1_data & (1 << 1) || g_client1_data & (1 << 2)) {
				// 왼쪽 가이드 라인 생성
				ellipse(img, Point(0, 240), Size(220.0, 70.0), 90, 0, 360, Scalar(0, 0, 255), 5, 8);
			}
			if (g_client1_data & (1 << 3) || g_client2_data & (1 << 4)) {
				// 앞쪽 가이드 라인 생성
				ellipse(img, Point(320, 0), Size(300.0, 70.0), 180, 0, 360, Scalar(0, 0, 255), 5, 8);
			}
			if (g_client2_data & (1 << 5) || g_client2_data & (1 << 6)) {
				// 오른쪽 가이드 라인 생성
				ellipse(img, Point(640, 240), Size(220.0, 70.0), 90, 0, 360, Scalar(0, 0, 255), 5, 8);
			}
		}
		imshow("camera", img);
		if (waitKey(1) == 27)
			break;
	}
}

static void ft_pthread_init(client_t client_data[], pthread_t client_thread[]
	pthread_t camera_control_thread, pthread_t piezo_control_thread)
{
	pthread_create(&client_thread[0], NULL, client_thread_create, &client_data[0]);
	pthread_create(&client_thread[1], NULL, client_thread_create, &client_data[1]);
	pthread_create(&camera_control_thread, NULL, camera_control, NULL);
	pthread_create(&piezo_control_thread, NULL, piezo_control, NULL);

	pthread_join(client_thread[0], NULL);
	pthread_join(client_thread[1], NULL);
	pthread_join(camera_control_thread, NULL);
	pthread_join(piezo_control_thread, NULL);
}

static void close_sock(client_t client_data[], int serv_sock)
{
	close(client_data[0].fd);
	close(client_data[1].fd);
	close(serv_sock);
}

int main(void) {
	int serv_sock, clnt_sock_fd;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;
	client_t client_data[2];
	pthread_t client_thread[2];
	pthread_t camera_control_thread, piezo_control_thread;

	serv_sock = server_sock_open();

	clnt_addr_size = sizeof(clnt_addr);
	for (int i = 0; i < 2; i++) {
		clnt_sock_fd = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if (clnt_sock_fd == -1) error_handling("accept() error");
		client_data[i].fd = clnt_sock_fd;
		strcpy(client_data[i].ip, inet_ntoa(clnt_addr.sin_addr));
	}

	ft_pthread_init(client_data, client_thread, camera_control_thread, piezo_control_thread);

	close_sock(client_data, serv_sock);

	return(0);
}
