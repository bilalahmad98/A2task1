#include <stdio.h> // basic I/O
#include <stdlib.h>
#include <sys/types.h> // standard system types
#include <netinet/in.h> // Internet address structures
#include <sys/socket.h> // socket API
#include <arpa/inet.h>
#include <netdb.h> // host to IP resolution
#include <string.h>
#include <unistd.h>
#include <iostream>

using namespace std;

#define HOSTNAMELEN 40 // maximal host name length; can make it variable if you want
#define BUFLEN 1024 // maximum response size; can make it variable if you want
#define BUFFER_SIZE 256
// prototype
void parametercheck(int argc, char *argv[]);
int openSocket(void);
void connectwithserver(int socket_id, struct sockaddr_in srv_ad);
struct hostent* getHostName(char* nameOhost);
void RcvMsg(int socket_id, char buffer[], int buffer_size);
void sendMsg(int socket_id, char buffer[], int buffer_size);
void clearbuff(char buffer[]);

int main(int argc, char *argv[])
{
  	// variable definitions
	int c_sock_fd, port;
	struct sockaddr_in srv_ad;
	struct hostent *serv;
	char buffer[BUFFER_SIZE];

  	// check that there are enough parameters
 	parametercheck(argc, argv);

    port = atoi(argv[2]);	// getting port no.

	c_sock_fd = openSocket();	// open socket
	
	char *nameOhost = argv[1];	
	serv = getHostName(nameOhost);	// get nameOhost

	// server details
	srv_ad.sin_family = AF_INET;
	srv_ad.sin_port = htons(port);
	int serv_l = serv->h_length;
	bcopy((char *)serv->h_addr, (char *) &srv_ad.sin_addr.s_addr, serv_l);
	
	connectwithserver(c_sock_fd, srv_ad);	// Connect to server

	clearBuff(buffer);
	
	RcvMsg(c_sock_fd, buffer, BUFFER_SIZE);

	cout << buffer << endl;
	
	bool isTrue = true;
	while (isTrue)
	{
		clearBuff(buffer);

		cout << "c : ";
		fgets(buffer, BUFFER_SIZE, stdin);
		buffer[strlen(buffer)] = '\r';
		buffer[strlen(buffer) + 1] = '\n';
		buffer[strlen(buffer) + 2] = '\0';

		sendMsg(c_sock_fd, buffer, strlen(buffer));

		clearBuff(buffer);

		RcvMsg(c_sock_fd, buffer, BUFFER_SIZE);

		cout << buffer << endl;
	}

	close(c_sock_fd);

  return 0;
}

void parametercheck(int argc, char* argv[])
{
	if (argc != 3)
    {
      fprintf(stderr, "Usage: %s <nameOhost> <port>\n", argv[0]);
      exit(-1);
    }
}

int openSocket(void)
{
	int socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_id < 0)
	{
		cout << "Socket Opening Failed\n";
		exit(-1);
	}
	return socket_id;
}

void connectwithserver(int socket_id, struct sockaddr_in srv_ad)
{
	int srv_size = sizeof(srv_ad);
	if (connect(socket_id, (struct sockaddr *) &srv_ad, srv_size) < 0)
	{
		cout << "Connection Failed\n";
		exit(-1);
	}
}

struct hostent* getHostName(char* nameOhost)
{
	struct hostent *serv;
	serv = getHostName(nameOhost);
	if (serv == NULL)
	{
		cout << "Error: Host Unavailable\n";
		exit(-1);
	}
	return serv;
}

void RcvMsg(int socket_id, char buffer[], int buffer_size)
{
	int x = read(socket_id, buffer, buffer_size);
	if (x < 0)
	{
		cout << "Failure of reading from socket\n";
		exit(-1);
	}
}

void sendMsg(int socket_id, char buffer[], int buffer_size)
{
	int x = write(socket_id, buffer, buffer_size);
	if (x < 0)
	{
		cout << "Failure of writing to socket\n";
		exit(-1);
	}
}

void clearBuff(char buffer[])
{
	bzero(buffer, BUFFER_SIZE);	
}