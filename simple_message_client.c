/**
 *
 * ./client -s localhost -p 6819 -u ic14b070 -m Hallo
 * simple_message_server -p 6819
 * nc localhost 6819
 *
 * @file simple_message_client.c
 * Simple Message Client
 *
 * @author Andreas Rubik <andreas.rubik@technikum-wien.at>
 * @author Anna Glock <ic14b070@technikum-wien.at>
 *
 * @version 1.0
 * @date 2015/11/18
 *
 */

 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <simple_message_client_commandline_handling.h>

#define SERVERPORT 5000+1819
#define BUFSIZE 1024

void usage(FILE *stream, const char *cmnd, int exitcode);
void error(char *msg);


//http://stackoverflow.com/questions/14562845/why-does-passing-char-as-const-char-generate-a-warning
int main(int argc, const char * const argv[]) {
	//printf("%d ... %s\n", argc,argv[0]);
	//usage(FILE *stream, const char *cmnd, int exitcode);
	//http://stackoverflow.com/questions/840501/how-do-function-pointers-in-c-work
	/*void (* smc_usagefunc_t) (FILE *, const char *, int);
	smc_usagefunc_t = &usage;*/
	int sockfd,n;
	//char buf[BUFSIZE];
	/*
	struct sockaddr_in {
        short   sin_family;
        u_short sin_port;
        struct  in_addr sin_addr;
        char    sin_zero[8];
	};
	*/
	//struct sockaddr_in serv_addr;
	//!!!!!!!!!error handling!!!!!!
	//int portno=SERVERPORT;
	
	const char *server="localhost";
    const char *port="6783";
	const char *user="ic14b070";
    const char *message="Hallo Welt!";
    const char *img_url=" ";
    int verbose=-1;
	void (* smc_usagefunc_t) (FILE *, const char *, int);
	smc_usagefunc_t=&usage;
    struct sockaddr_in serveraddr;
	
	struct hostent *serverHostent;
	
	/*
	void smc_parsecommandline(
           int argc,
           const char * const argv[],
           smc_usagefunc_t usage,
           const char **server,
           const char **port,
           const char **user,
           const char **message,
           const char **img_url,
           int *verbose
           );
	*/
	
	//struct addrinfo serverHint, *servinfo;
	
	smc_parsecommandline(argc,argv,smc_usagefunc_t,&server,&port,&user,&message,&img_url,&verbose);
	
	printf("%s\n",port);
	
	 
	//servinfo->ai_family= AF_INET;  
    //servinfo->ai_socktype = SOCK_STREAM;
	
	
	//getaddrinfo(server, port, &serverHint, &servinfo);
	
	serverHostent = gethostbyname(server);
    if (serverHostent == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
	
	
	bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)serverHostent->h_addr, 
		  (char *)&serveraddr.sin_addr.s_addr, serverHostent->h_length);
    serveraddr.sin_port = htons(atoi(port));
	
	printf("%d\n",serveraddr.sin_port);
	
	sockfd = socket(serveraddr.sin_family, SOCK_STREAM, 0); 
     if (sockfd < 0) {		 
        error("ERROR opening socket");
	 }
	 
	

	//const struct sockaddr serverAdd;
	
	//bzero((char *) &serverAdd, sizeof(serverAdd));
    //serverAdd.sa_family = AF_INET;
	
	
    /* connect: create a connection with the server */
    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
      error("ERROR connecting");
	}

  
    /* get message line from the user */
    //printf("Please enter msg: ");
    //bzero(buf, BUFSIZE);
    //fgets(buf, BUFSIZE, stdin);

	printf("Vor Write\n");
	printf("Message: %s\n",message);
    /* send the message line to the server */
    n = write(sockfd, message, strlen(message));
    if (n < 0) 
      error("ERROR writing to socket");

    /* print the server's reply */
    /*bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("Echo from server: %s", buf);
    close(sockfd);*/
	
	printf("erfolgereich mit Server kommuniziert.\n");
	
    return EXIT_SUCCESS;
}

// getLine um line vom Socket zu bekommen ... speicher frei geben

void usage(FILE *stream, const char *cmnd, int exitcode){
	printf("ALLES KAPUTT: %d%d%d",stream->_flags,*cmnd,exitcode);
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}