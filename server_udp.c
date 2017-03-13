#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 9600
#define BUF_SIZE 128

int main(int argc, char *argv[]) {
	int server_fd = socket(PF_INET, SOCK_DGRAM, 0);
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

	printf("Server listening on port %d\n", PORT);

	char buf[BUF_SIZE];
	ssize_t n;
	struct sockaddr_in client_addr;
	socklen_t client_addr_len;
	while (1) {
		n = recvfrom(server_fd, &buf, sizeof(buf)-1, 0, (struct sockaddr *) &client_addr, &client_addr_len);
		if (n < 0) {
			fprintf(stderr, "Cannot read from socket\n");
			return 1;
		}
		if (n == 0) {
			continue; // EOF
		}
		buf[n] = '\0';

		printf("Received: %s\n", buf);
	}
}
