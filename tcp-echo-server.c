#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define on_error(...)                 \
    {                                 \
        fprintf(stderr, __VA_ARGS__); \
        perror("cause by");           \
        fflush(stderr);               \
        exit(1);                      \
    }

void echo(int client_fd)
{
    char buff[BUFFER_SIZE];
    do
    {
        int read_bytes = recv(client_fd, buff, BUFFER_SIZE, 0);
        if (read_bytes <= 0)
        {
            printf("Failed to read client");
            break;
        }

        printf("Read from client %d: %s \n", client_fd, buff);
        if (send(client_fd, buff, read_bytes, 0) < 0)
        {
            printf("Failed to write client");
            break;
        }
        bzero(buff, read_bytes);
    } while (strncmp(buff, "bye\r", 4) != 0);
    printf("Connection closed: %d\n", client_fd);
    close(client_fd);
}

int main(int argc, char *argv[])
{
    /*
     * check command line arguments
     */
    if (argc < 2)
        on_error("Usage: %s [port]\n", argv[0]);
    int port = atoi(argv[1]);

    int server_fd, client_fd;                  /* server and client file descriptor*/
    struct sockaddr_in serveraddr, clientaddr; /* server and client address*/
    socklen_t client_len = sizeof(clientaddr); /* byte size of client's address */

    /*
     * socket: create socket
     */
    server_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        on_error("Could not create socket\n");

    /*
     * build the server's Internet address
     */
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;                /* this is an IPv4 Internet address */
    serveraddr.sin_port = htons(port);              /* this is the port we will listen on */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); /* let the system figure out our IP address */

    /* setsockopt: Handy debugging trick that lets
     * us rerun the server immediately after we kill it;
     * otherwise we have to wait about 20 secs.
     * Eliminates "ERROR on binding: Address already in use" error.
     */
    int opt_val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);

    /*
     * bind: associate socket with a port
     */
    if (bind(server_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        on_error("Could not bind socket\n");

    /*
     * listen: make this socket ready to accept connection requests
     * allow 16 requests to queue up
     */
    if (listen(server_fd, 16) < 0)
        on_error("Could not listen on socket\n");

    printf("Server is listening on %d\n", port);

    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&clientaddr, &client_len);
        if (client_fd < 0)
            on_error("Could not establish new connection\n");
        printf("Connected: %s:%d, file descriptor: %d\n",
               inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), client_fd);

        echo(client_fd);
    }

    return 0;
}
