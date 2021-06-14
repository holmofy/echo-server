CC=cc

all: bin/tcp-echo-server \
	bin/tcp-echo-server-multithread \
	bin/tcp-non-blocking-poll-echo-server \
	bin/tcp-non-blocking-select-echo-server \
	bin/tcp-non-blocking-kqueue-echo-server \
	bin/tcp-non-blocking-libevent-echo-server \
	#bin/tcp-non-blocking-epoll-echo-server 

bin/tcp-echo-server: tcp-echo-server.c
	${CC} tcp-echo-server.c \
	-g \
	-o bin/tcp-echo-server

bin/tcp-echo-server-multithread: tcp-echo-server-multithread.c
	${CC} tcp-echo-server-multithread.c \
	-g \
	-o bin/tcp-echo-server-multithread

bin/tcp-non-blocking-poll-echo-server: tcp-non-blocking-poll-echo-server.c
	${CC} tcp-non-blocking-poll-echo-server.c \
	-g \
	-o bin/tcp-non-blocking-poll-echo-server

bin/tcp-non-blocking-select-echo-server: tcp-non-blocking-select-echo-server.c
	${CC} tcp-non-blocking-select-echo-server.c \
	-g \
	-o bin/tcp-non-blocking-select-echo-server

bin/tcp-non-blocking-epoll-echo-server: tcp-non-blocking-epoll-echo-server.c
	${CC} tcp-non-blocking-epoll-echo-server.c \
	-g \
	-o bin/tcp-non-blocking-epoll-echo-server

bin/tcp-non-blocking-kqueue-echo-server: tcp-non-blocking-kqueue-echo-server.c
	${CC} tcp-non-blocking-kqueue-echo-server.c \
	-g \
	-o bin/tcp-non-blocking-kqueue-echo-server

bin/tcp-non-blocking-libevent-echo-server: tcp-non-blocking-libevent-echo-server.c
	gCC-11 tcp-non-blocking-libevent-echo-server.c \
	-g \
	-I /usr/local/include \
	-L /usr/local/lib \
	-levent \
	-o bin/tcp-non-blocking-libevent-echo-server

clean:
	rm -r bin/*
