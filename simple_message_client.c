/**
 *
 * ./client -s localhost -p 6819 -u ic14b070 -m Hallo
 * simple_message_server -p 6819
 * nc localhost 6819
 *6795
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
#include <unistd.h>
#include <time.h>
#include <simple_message_client_commandline_handling.h>

#define SERVERPORT 5000+1819
#define BUFSIZE 1024

void usage(FILE *stream, const char *cmnd, int exitcode);
void error(char *msg);
void writeToServer(int sockfd, char * sendMessage, int messageLen);
char * makeSendMessage(const char *user,const char *message,const char *img_url);

int main(int argc, const char * const argv[]) {
	int sockfd;	
	const char *server="localhost";
    const char *port="6783";
	const char *user="ic14b070";
    const char *message="Hallo Welt!";
    const char *img_url="";
    int verbose=-1;
	void (* smc_usagefunc_t) (FILE *, const char *, int);
	smc_usagefunc_t=&usage;
    struct sockaddr_in serveraddr;	
	char *sendMessage="";	
	struct hostent *serverHostent;
	
	smc_parsecommandline(argc,argv,smc_usagefunc_t,&server,&port,&user,&message,&img_url,&verbose);
	
	serverHostent = gethostbyname(server);
    if (serverHostent == NULL) {
        error("ERROR, no such host\n");
    }
	
	
	bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)serverHostent->h_addr, 
		  (char *)&serveraddr.sin_addr.s_addr, serverHostent->h_length);
    serveraddr.sin_port = htons(atoi(port));
	
	sockfd = socket(serveraddr.sin_family, SOCK_STREAM, 0); 
     if (sockfd < 0) {		 
        error("ERROR opening socket");
	 }
	 		
    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
      error("ERROR connecting");
	}
  
	sendMessage = makeSendMessage(user,message,img_url);
	
	int messLen = strlen(sendMessage);
	int lenArlreadySend = 0;
	char *partMessage= (char *) malloc(255);
	int first=1;
	char buffer[2]= {'\0'};
	do{
		strncpy(partMessage,sendMessage+lenArlreadySend,254);
		writeToServer(sockfd, partMessage, strlen(partMessage));
		lenArlreadySend+=254;
		while(!first && buffer[0] == 0x12){
			int n;
			n = read(sockfd,buffer,2);
			if (n < 0){ 
				error("ERROR reading from socket");
			}
			struct timespec *req;
			struct timespec *rem;
			bzero((char *) &req, sizeof(req));
			req->tv_sec = (1 / 1000);
			req->tv_nsec = ((1 % 1000) * 1000000);
			if(nanosleep(req,rem)){
				error("ERROR waiting");
			}
		}
		first=0;
	}while(lenArlreadySend < messLen);

	if(shutdown(sockfd,2)<0){
		error("ERROR closing Socket");
	}
	
	free(sendMessage);
		
	
	
    return EXIT_SUCCESS;
}


void usage(FILE *stream, const char *cmnd, int exitcode){
	//",stream->_flags,*cmnd,exitcode	
	if (printf ("Error: %d; %d; %d\n", stream->_flags,*cmnd,exitcode) < 0){
		error("ERROR at writing Error.");
	}
	error("Usage: simple_message_client  -s server -p port -u user [-i image URL] -m message [-v] [-h]");
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void writeToServer(int sockfd, char * sendMessage, int messageLen){
	int n = write(sockfd, sendMessage, messageLen);
	if (n < 0) {
		error("ERROR writing to socket");
	}
}

char * makeSendMessage(const char *user,const char *message,const char *img_url){
	int len = 1 + 5 + strlen(user) + 1 + strlen(message) + (img_url != NULL ? 1 + 4 + strlen(img_url) : 0);
	char *sendMessage = (char *) malloc(len);
	strcpy(sendMessage, "user=");
	strcat(sendMessage, user);
	if(img_url != NULL){
		strcat(sendMessage, "\x0a");
		strcat(sendMessage, "img=");
		strcat(sendMessage, img_url);
	}
	strcat(sendMessage, "\x0a");
	strcat(sendMessage, message);
	
	return sendMessage;
}
