#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define on_error(...) {           \
    fprintf(stderr, __VA_ARGS__); \
    fflush(stderr);               \
    exit(1);                      \
}

void* client_fd_process(void* arg) {
    char buf[BUFFER_SIZE];
    int read_bytes;
    int client_fd = *(int *)arg;
    int err;
    do
    {
        read_bytes = recv(client_fd, buf, BUFFER_SIZE, 0);
        if(read_bytes < 0) break;

        printf("Read from client %d: %s \n", client_fd, buf);
        err = send(client_fd, buf, read_bytes, 0);
        if(err < 0) break;
    }
    while (strncmp(buf, "bye\r", 4) != 0);
    printf("Connection closed: %d\n", client_fd);
    close(client_fd);
    return arg;
}

int main(int argc, char *argv[]) {
    if (argc < 2) on_error("Usage: %s [port]\n", argv[0]);
    int port = atoi(argv[1]);

    int server_fd, err;
    struct sockaddr_in server, client;

    server_fd = socket(PF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) on_error("Could not create socket\n");

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    err = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
    if (err < 0) on_error("Could not bind socket\n");

    err = listen(server_fd, 16);
    if (err < 0) on_error("Could not listen on socket\n");

    printf("Server is listening on %d\n", port);

    while (1) {
        int client_fd;
        socklen_t client_len = sizeof(client);
        pthread_t child;

        client_fd = accept(server_fd, (struct sockaddr*)&client, &client_len);
        printf("Connected: %s:%d, fd: %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), client_fd);
        if ( pthread_create(&child, NULL, client_fd_process, &client_fd) != 0 )
            perror("Thread creation");
        else
            pthread_detach(child);  /* disassociate from parent */
    }
    return 0;
}

