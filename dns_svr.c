// A simple server in the internet domain using TCP
// The port number is passed as an argument

//adapted from workshop 9 code 
#define _POSIX_C_SOURCE 200112L
#define PORT "8053"
#define INFINITE 1
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "phase1.h"
#include "packet.h"
#include "header.h"
#include "question.h"
#include "answer.h"

struct dns_message *communicate_upstream(char **argv, struct dns_message *message);

//adapted from workshop 9 code 
int main(int argc, char** argv) {
	int sockfd, newsockfd, n, re, s;
	struct addrinfo hints, *res;
	struct sockaddr_storage client_addr;
	socklen_t client_addr_size;


	// Create address we're going to listen on (with given port number)
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;       // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = AI_PASSIVE;     // for bind, listen, accept
	
	// node (NULL means any interface), service (port), hints, res
	//converts human-readable text strings representing hostnames or IP addresses into a dynamically allocated linked list of struct addrinfo structures.
	s = getaddrinfo(NULL, PORT, &hints, &res);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	// Create socket on computer
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// Reuse port if possible
	re = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &re, sizeof(int)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	
	// Bind address to the socket created
	if (bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(res);

	// Listen on socket - means we're ready to accept connections,
	// incoming connection requests will be queued
	if (listen(sockfd, 5) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// Accept a connection - blocks until a connection is ready to be accepted
	// Get back a new file descriptor to communicate on
	//newsockfd is used to accept the connection that is created previously defined in sockfd
	//get socket for client
	while(INFINITE){
		client_addr_size = sizeof client_addr;
		newsockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_size);
		if (newsockfd < 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}                                                                                                                                                       

	
		//reading in from the client into dns svr
		struct dns_message *message = parse(newsockfd); 

		//if the request is not a AAAA query, set the rcode to 4 and create a packet 
		if(message -> question -> qtype != AAAA){
			//create an error packet if the message is not an AAAA query
			struct error_packet *error_response = create_response(message);
			// Write back to the client
			n = write(newsockfd, error_response -> header, error_response -> buffer_length);
			if (n < 0) {
				perror("write");
				exit(EXIT_FAILURE);
			}
		}
		//if the request is a AAAA query
		else{
			//send the message and receive the response from the upstream server
			struct dns_message *response = communicate_upstream(argv, message);
			// Writing back to client
			n = write(newsockfd, response -> buffer, response -> buffer_length);
			if (n < 0) {
				perror("write");
				exit(EXIT_FAILURE);
			}
		}
			
	}

	//close socket
	close(sockfd);
	close(newsockfd);
	

	return 0;
}





//send and receive response from the upstream server
//adapted from workshop 9 code 
struct dns_message *communicate_upstream(char **argv, struct dns_message *message){
	int sockfd, n, s;
	struct addrinfo hints, *servinfo, *rp;
	char *ip_address = argv[1];
	char *port = argv[2];

	// Create address
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	// Get addrinfo of server. From man page:
	// The getaddrinfo() function combines the functionality provided by the
	// gethostbyname(3) and getservbyname(3) functions into a single interface
	s = getaddrinfo(ip_address, port, &hints, &servinfo);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}


	// Connect to first valid result
	// Why are there multiple results? see man page (search 'several reasons')
	// How to search? enter /, then text to search for, press n/N to navigate
	for (rp = servinfo; rp != NULL; rp = rp->ai_next) {
		sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sockfd == -1){
			continue;
		}

		if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break; // success

		close(sockfd);
	}

	if (rp == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(servinfo);

	//Send message to the upstream server
	n = write(sockfd, message -> buffer, message -> buffer_length);
	if (n < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	//from sockfd into buffer
	//Receiving a response from the upstream server
	struct dns_message *response = parse(sockfd);
	

	close(sockfd);


	return response;
}
