#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define tcp_socket() socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
#define IPPORT_HTTP 80
#define REQUEST "GET /\n"

int main(int argc, char *argv[])
{
	char buf[BUFSIZ];
	struct hostent *host_ent;
	int sock;
	struct sockaddr_in addr;
	int rsize;
   
	printf("Enter the domain: ");
	scanf("%s", buf);

	host_ent = gethostbyname(buf);
	if (!host_ent)
	{
		perror("gethostbyname");
		exit(EXIT_FAILURE);
	}

	sock = tcp_socket();
	if (sock == -1)
	{
		perror("tcp socket");
		exit(EXIT_FAILURE);
	}
	addr.sin_family = AF_INET;
	addr.sin_addr = *( (struct in_addr *)host_ent->h_addr);
	addr.sin_port = htons(IPPORT_HTTP);
	if ( connect(sock, (struct sockaddr *)&addr, sizeof(addr) ) == -1)
	{
		perror("connect");
		exit(EXIT_FAILURE);
	}

	if ( send(sock, REQUEST, strlen(REQUEST), 0) == -1)
	{
		perror("send");
		exit(EXIT_FAILURE);
	}
	for (;;) {
		rsize = recv(sock, buf, sizeof(buf), 0);
		if (rsize <= 0)
		{
			if (rsize == -1)
			{
				perror("recv");
				exit(EXIT_FAILURE);
			}
			break;
		}
		printf("%s", buf);
	}
	printf("\n");

	close(sock);
	exit(EXIT_SUCCESS);
}
