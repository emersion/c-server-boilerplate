#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 9600
#define BUF_SIZE 100

int main(int argc, char *argv[]) {
	int client_fd = socket(PF_INET, SOCK_DGRAM, 0);
	if (client_fd < 0) {
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

	int err = connect(client_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if (err != 0) {
		fprintf(stderr, "Cannot connect to server\n");
		return 1;
	}

	char *msg = "Hello World!";
	ssize_t n = write(client_fd, msg, strlen(msg)+1);
	if (n < 0) {
		fprintf(stderr, "Cannot write data\n");
		return 1;
	}

	printf("Message sent!\n");
	return 0;
}
