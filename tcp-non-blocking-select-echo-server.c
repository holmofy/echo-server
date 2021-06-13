#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>

#define PORT "9421"

/* function prototypes */
void die(const char *);

int main(int argc, char **argv)
{
    int sockfd, new, maxfd, on = 1, nready, i;

    struct addrinfo *res0, *res, hints;

    char buffer[BUFSIZ];

    fd_set master, readfds;

    int error;

    ssize_t nbytes;

    (void)memset(&hints, '\0', sizeof(struct addrinfo));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if (0 != (error = getaddrinfo(NULL, PORT, &hints, &res0)))
        errx(EXIT_FAILURE, "%s", gai_strerror(error));

    for (res = res0; res; res = res->ai_next)
    {
        if (-1 == (sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)))
        {
            perror("socket()");
            continue;
        }

        if (-1 == (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(int))))
        {
            perror("setsockopt()");
            continue;
        }

        if (-1 == (bind(sockfd, res->ai_addr, res->ai_addrlen)))
        {
            perror("bind()");
            continue;
        }

        break;
    }

    if (-1 == sockfd)
        exit(EXIT_FAILURE);

    freeaddrinfo(res0);

    if (-1 == (listen(sockfd, 32)))
        die("listen()");

    if (-1 == (fcntl(sockfd, F_SETFD, O_NONBLOCK)))
        die("fcntl()");

    FD_ZERO(&master);
    FD_ZERO(&readfds);

    FD_SET(sockfd, &master);

    maxfd = sockfd;

    while (1)
    {
        memcpy(&readfds, &master, sizeof(master));

        (void)printf("running select()\n");

        if (-1 == (nready = select(maxfd + 1, &readfds, NULL, NULL, NULL)))
            die("select()");

        (void)printf("Number of ready descriptor: %d\n", nready);

        for (i = 0; i <= maxfd && nready > 0; i++)
        {
            if (FD_ISSET(i, &readfds))
            {
                nready--;

                if (i == sockfd)
                {
                    (void)printf("Trying to accept() new connection(s)\n");

                    if (-1 == (new = accept(sockfd, NULL, NULL)))
                    {
                        if (EWOULDBLOCK != errno)
                            die("accept()");

                        break;
                    }

                    else
                    {

                        if (-1 == (fcntl(new, F_SETFD, O_NONBLOCK)))
                            die("fcntl()");

                        FD_SET(new, &master);

                        printf("accept new client fd:%d\n", new);

                        if (maxfd < new)
                            maxfd = new;
                    }
                }

                else
                {
                    (void)printf("recv() data from fd:%d\n", i);

                    nbytes = recv(i, buffer, sizeof(buffer), 0);
                    if (nbytes <= 0)
                    {
                        if (EWOULDBLOCK != errno)
                            die("recv()");

                        break;
                    }

                    buffer[nbytes] = '\0';
                    printf("%s", buffer);
                    (void)printf("%zi bytes received.\n", nbytes);

                    send(i, buffer, nbytes, 0);

                    // close(i);
                    // FD_CLR(i, &master);
                }
            }
        }
    }
    return 0;
}

void die(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}