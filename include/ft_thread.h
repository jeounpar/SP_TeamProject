#ifndef FT_THREAD_H
# define FT_THREAD_H

typedef struct client_s{
	int fd;
	char ip[30];
} client_t;

#endif