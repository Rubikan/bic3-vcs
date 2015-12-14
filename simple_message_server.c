/**
 *
 * @file simple_message_server.c
 * Simple Message Server
 *
 * @author Andreas Rubik <andreas.rubik@technikum-wien.at>
 * @author Anna Glock <ic14b070@technikum-wien.at>
 *
 * @version 1.0
 * @date 2015/11/18
 *
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define DEFAULT_PORT 5000+1819

void usage(void);
void splitMessage(char charArr[]);

int main(int argc, char* argv[]) {
    int serverSocketID;
    int connectedClient;
    struct sockaddr_in serverSocketAdress;
    struct sockaddr_in clientSocketAdress;
    unsigned short int port;

    if (argc > 3) {
        usage();
        return EXIT_FAILURE;
    }

    int o;
    while ((o = getopt(argc, argv, "p:h")) != -1) {
        switch(o) {
            case 'p':
                errno = 0;
                port = atoi(optarg);
                break;
            case 'h':
                usage();
                break;
            default:
                usage();
                exit(EXIT_FAILURE);
        }
    }

    // If no port is given, use the default port
    if (argc == 1) {
        port = DEFAULT_PORT;
    }

    serverSocketID = socket(AF_INET, SOCK_STREAM, 0);

    // Adressen vom Internet
    serverSocketAdress.sin_family = AF_INET;
    // Es werden Verbindungen von allen Adressen akzeptiert
    serverSocketAdress.sin_addr.s_addr = INADDR_ANY;
    // Serverport festlegen
    serverSocketAdress.sin_port = htons(port);

    if (bind(serverSocketID, (struct sockaddr*)&serverSocketAdress, sizeof(serverSocketAdress)) < 0) {
        fprintf(stderr, "Error binding socket!\n%s", strerror(errno));
        return EXIT_FAILURE;
    }
    if (listen(serverSocketID, 5) == -1) {
        fprintf(stderr, "Error listening on socket!\n%s", strerror(errno));
        return EXIT_FAILURE;
    }

    for(;;) {
        int c = sizeof(struct sockaddr_in);
        connectedClient = accept(serverSocketID, (struct sockaddr*)&clientSocketAdress, (socklen_t*)&c);
        if (connectedClient < 0) {
            if (errno == EINTR) {
                // accept was interrupted, trying again
                continue;
            } else {
                fprintf(stderr, "Error accepting incoming connection!\n%s", strerror(errno));
                return EXIT_FAILURE;
            }
        }

        /*printf("TEST: %d\n", port);

        // read 256 bytes that the client send
        // need to check the last bit if more is comming
        // ka was für ein zeichen das dann wird
        char buffer[256]= {'\0'};
        int n;
        n = read(connectedClient,buffer,255);
        if (n < 0){
            perror("ERROR reading from socket");
            return EXIT_FAILURE;
        }

        printf("Here is the message: %s\n",buffer);

        splitMessage(buffer);*/

        int child = fork();
        if (child == -1) {
            // Fork Failed
            close(serverSocketID);
            close(connectedClient);
            fprintf(stderr, "Error executing fork!\n%s", strerror(errno));
            return EXIT_FAILURE;
        } else if (child == 0) {
            // Server socket is not needed anymore
            close(serverSocketID);
            // Replace stdin with the new socket descriptor
            if (dup2(connectedClient, 0) == -1) {
                fprintf(stderr, "Error redirecting stdin to socket descriptor!\n%s", strerror(errno));
                close(connectedClient);
                return EXIT_FAILURE;
            }
            // Replace stdout with the new socket descriptor
            if (dup2(connectedClient, 1) == -1) {
                fprintf(stderr, "Error redirecting stdout to socket descriptor!\n%s", strerror(errno));
                close(connectedClient);
                return EXIT_FAILURE;
            }
            // Execute the business logic
            if (execlp("/usr/local/bin/simple_message_server_logic", "simple_message_server_logic", NULL) == -1) {
                fprintf(stderr, "Error executing business logic!\n%s", strerror(errno));
            }
        }

        close(connectedClient);
    }

    return EXIT_SUCCESS;
}

void usage() {
    perror("Usage: simple_message_server [-p port]\\n");
}

/*void splitMessage(char charArr[]){
	char userPartOfMessage[256]= {'\0'};
	char restPartOfMessage[256]= {'a','b','c','\0'};
	int i=0; 
	int count=0;
	int lenUser=1;
	for(i=0;charArr[i]!='\0';i++){
		char c = charArr[i];
		if(c != 0x0a || (c == 0x0a && charArr[i+1] == 0x0d)){
			if(count == 0){					
				userPartOfMessage[i]=c;
				lenUser++;
			} else {
				restPartOfMessage[i-lenUser]=c;
			}
		} else {
			printf("Trennzeichen gefunden JUHU!!!!\n");
			printf("next char: %c\n",charArr[i+1]);
			count++;
		}
	}
	printf("User: %s\n", userPartOfMessage);
	printf("Rest: %s\n", restPartOfMessage);
}*/
