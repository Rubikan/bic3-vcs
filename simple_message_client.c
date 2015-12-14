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


//http://stackoverflow.com/questions/14562845/why-does-passing-char-as-const-char-generate-a-warning
int main(int argc, const char * const argv[]) {
	//printf("%d ... %s\n", argc,argv[0]);
	//usage(FILE *stream, const char *cmnd, int exitcode);
	//http://stackoverflow.com/questions/840501/how-do-function-pointers-in-c-work
	/*void (* smc_usagefunc_t) (FILE *, const char *, int);
	smc_usagefunc_t = &usage;*/
	int sockfd;
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
    const char *img_url="";
    int verbose=-1;
	void (* smc_usagefunc_t) (FILE *, const char *, int);
	smc_usagefunc_t=&usage;
    struct sockaddr_in serveraddr;
	
	char *sendMessage="";
	
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
        error("ERROR, no such host\n");
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
	
	sendMessage = makeSendMessage(user,message,img_url);
	
	printf("whole Message: %s\n",sendMessage);
	int messLen = strlen(sendMessage);
	//if(messLen<256){
	//	writeToServer(sockfd, sendMessage, strlen(sendMessage));
	//} else {
		//int anzAnWrites = (int) ((messLen / 255)+0.5);
		int lenArlreadySend = 0;
		char *partMessage= (char *) malloc(255);
		int first=1;
		char buffer[2]= {'\0'};
		do{
			strncpy(partMessage,sendMessage+lenArlreadySend,254);
			writeToServer(sockfd, partMessage, strlen(partMessage));
			while(!first && buffer[0] == 0x12){
				//waitForServer()
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
	//}

	if(shutdown(sockfd,2)<0){
		error("ERROR closing Socket");
	}
    /* print the server's reply */
    /*bzero(buf, BUFSIZE);
    n = read(sockfd, buf, BUFSIZE);
    if (n < 0) 
      error("ERROR reading from socket");
    printf("Echo from server: %s", buf);
    close(sockfd);*/
	
	free(sendMessage);
	
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

void writeToServer(int sockfd, char * sendMessage, int messageLen){
		/* send the message line to the server */
	int n = write(sockfd, sendMessage, messageLen);
	if (n < 0) {
		error("ERROR writing to socket");
	}
}

char * makeSendMessage(const char *user,const char *message,const char *img_url){
	printf("Start\n");
	//char * str3 = (char *) malloc(1 + strlen(str1)+ strlen(str2) );
	printf("user: %d\n",strlen(user));
	printf("message: %d\n",strlen(message));
	if(img_url != NULL){
		printf("img_url: %d\n",strlen(img_url));
	} else {
		printf("img_url is NULL\n");
	}
	int len = 1 + 5 + strlen(user) + 1 + strlen(message) + (img_url != NULL ? 1 + 4 + strlen(img_url) : 0);
	printf("len: %d\n",len);
	char *sendMessage = (char *) malloc(len);
	printf("sendMessage: %s\n",sendMessage);
	strcpy(sendMessage, "user=");
	strcat(sendMessage, user);
	//sendMessage+="user="
	//sendMessage+=user;
	if(img_url != NULL){
		strcat(sendMessage, "\x0a");
		strcat(sendMessage, "img=");
		strcat(sendMessage, img_url);
		//sendMessage+="\x0a"+"img="+img_url;
	}
	strcat(sendMessage, "\x0a");
	strcat(sendMessage, message);
	//sendMessage+="\x0a"+message;
	
	printf("sendMessage: %s\n",sendMessage);
	return sendMessage;
}