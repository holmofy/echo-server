all: bin/tcp-echo-server \
	bin/tcp-echo-server-multithread \
	bin/tcp-non-blocking-poll-echo-server \
	bin/tcp-non-blocking-select-echo-server \
	bin/tcp-non-blocking-kqueue-echo-server \
	#bin/tcp-non-blocking-epoll-echo-server \

bin/tcp-echo-server: tcp-echo-server.c
	cc tcp-echo-server.c -o bin/tcp-echo-server

bin/tcp-echo-server-multithread: tcp-echo-server-multithread.c
	cc tcp-echo-server-multithread.c -o bin/tcp-echo-server-multithread

bin/tcp-non-blocking-poll-echo-server: tcp-non-blocking-poll-echo-server.c
	cc tcp-non-blocking-poll-echo-server.c -o bin/tcp-non-blocking-poll-echo-server

bin/tcp-non-blocking-select-echo-server: tcp-non-blocking-select-echo-server.c
	cc tcp-non-blocking-select-echo-server.c -o bin/tcp-non-blocking-select-echo-server

bin/tcp-non-blocking-epoll-echo-server: tcp-non-blocking-epoll-echo-server.c
	cc tcp-non-blocking-epoll-echo-server.c -o bin/tcp-non-blocking-epoll-echo-server

bin/tcp-non-blocking-kqueue-echo-server: tcp-non-blocking-kqueue-echo-server.c
	cc tcp-non-blocking-kqueue-echo-server.c -o bin/tcp-non-blocking-kqueue-echo-server

clean:
	rm bin/*
