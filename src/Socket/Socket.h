//
// Created by Antoine TOUZART on 19/11/22.
//

#ifndef CASSEBRIQUES_SOCKET_H
#define CASSEBRIQUES_SOCKET_H

int serverStart();
void closeServer(int new_socket);
void shutdownServer(int server_fd);
int clientStart(char *ipAddr);
void closeClient(int client_fd);

#endif // CASSEBRIQUES_SOCKET_H
