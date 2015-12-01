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
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define DEFAULT_PORT 5000+1819

void usage() {
    perror("Usage: simple_message_server [-p port]\\n");
}

int main(int argc, char* argv[]) {
    int serverSocketID;
    int connectedClient;
    struct sockaddr_in serverSocketAdress;
    struct sockaddr_in clientSocketAdress;
    unsigned short int port;

    if (argc > 2) {
        usage();
        return EXIT_FAILURE;
    }

    int opt;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch(opt) {
            case 'p':
                errno = 0;
                port = atoi(optarg);
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
        perror("Error binding socket!");
        return EXIT_FAILURE;
    }
    if (listen(serverSocketID, 5) == -1) {
        perror("Error listening on socket!");
        return EXIT_FAILURE;
    }

    int c = sizeof(struct sockaddr_in);
    connectedClient = accept(serverSocketID, (struct sockaddr*)&clientSocketAdress, (socklen_t*)&c);
    if (connectedClient < 0) {
        perror("Error accepting incoming connection!");
        return EXIT_FAILURE;
    }

    printf("TEST: %d", port);

    // Business Logic

    close(serverSocketID);

    return EXIT_SUCCESS;
}
