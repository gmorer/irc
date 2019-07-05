#include "server.h"

int error(char *message)
{
	perror(message);
	return (EXIT_FAILURE);
}

int init_server()
{
	struct sockaddr_in sockaddr;
	int sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return (error("Opening socket: "));
	bzero(&sockaddr, sizeof(struct sockaddr_in));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr.sin_port = htons(PORT);
	if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1)
		return (error("Binding socket: "));
	if (listen(sockfd, BACKLOG) == -1)
		return (error("Listening: "));
	return (sockfd);
}

int main(int argc, char **argv)
{
	struct sockaddr_in cliaddr;
	socklen_t clilen;
	int sockfd;
	int newfd;

	if ((sockfd = init_server()) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	printf("listening on port %d\n", PORT);
	server_loop(sockfd);
	return (EXIT_SUCCESS);
}
