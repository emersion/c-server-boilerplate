#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9600
#define BUF_SIZE 128
#define SERVER_BACKLOG 10

void *handle_conn(void *arg) {
	int conn_fd = *((int*) arg);

	char buf[BUF_SIZE];
	ssize_t n;
	while (1) {
		n = read(conn_fd, &buf, sizeof(buf)-1);
		if (n < 0) {
			fprintf(stderr, "Error while reading from connection [%d]\n", conn_fd);
			return NULL;
		}
		if (n == 0) {
			break;
		}
		buf[n] = '\0';

		printf("Received [%d]: %s\n", conn_fd,  buf);
	}

	printf("Closing connection [%d]\n",conn_fd);
	int err = close(conn_fd);
	if (err != 0) {
		fprintf(stderr, "Error while closing connection [%d]\n", conn_fd);
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	int server_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
		fprintf(stderr, "Cannot create socket\n");
		return 1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	int ok = inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
	if (!ok) {
		fprintf(stderr, "Cannot parse IP address\n");
		return 1;
	}

	int err = bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (err != 0) {
		fprintf(stderr, "Cannot bind server\n");
		return 1;
	}

	err = listen(server_fd, SERVER_BACKLOG);
	if (err != 0) {
		fprintf(stderr, "Cannot listen\n");
		return 1;
	}

	printf("Server listening on port %d\n", PORT);

	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t client_addr_len;
	pthread_t thread;
	while (1) {
		client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
		if (client_fd < 0) {
			fprintf(stderr, "Cannot accept\n");
			return 1;
		}

		printf("New client [%d]\n", client_fd);

		err = pthread_create(&thread, NULL, &handle_conn, (void*) &client_fd);
		if (err != 0) {
			fprintf(stderr, "Cannot create thread to handle client connection\n");
			return 1;
		}
	}
}
