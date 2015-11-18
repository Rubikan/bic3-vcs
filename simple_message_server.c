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
#include <stdlib.h>
#include <sys/socket.h>

#define SERVERPORT 5000+1819

int main(int argc, char* argv[]) {
    int serverSocketID;
    int connectedClient;
    struct sockaddr_in serverSocketAdress;
    struct sockaddr_in clientSocketAdress;

    serverSocketID = socket(AF_INET, SOCK_STREAM, 0);

    // Adressen vom Internet
    serverSocketAdress.sin_family = AF_INET;
    // Es werden Verbindungen von allen Adressen akzeptiert
    serverSocketAdress.sin_addr.s_addr = INADDR_ANY;
    // Serverport festlegen
    serverSocketAdress.sin_port = htons(SERVERPORT);

    if (bind(serverSocketID, &serverSocketAdress, sizeof(serverSocketAdress)) < 0) {
        perror("Error binding socket!");
        return EXIT_FAILURE;
    }
    if (listen(serverSocketID, 5) == -1) {
        perror("Error listening on socket!");
        return EXIT_FAILURE;
    }

    connectedClient = accept(serverSocketID, &clientSocketAdress, sizeof(clientSocketAdress));
    if (connectedClient < 0) {
        perror("Error accepting incoming connection!");
        return EXIT_FAILURE;
    }

    // Business Logic

    close(serverSocketID);

    return EXIT_SUCCESS;
}