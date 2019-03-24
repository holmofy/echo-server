all: bin/tcp-echo-server bin/tcp-non-blocking-echo-server bin/tcp-echo-server-multithread

bin/tcp-echo-server: tcp-echo-server.c
	cc tcp-echo-server.c -o bin/tcp-echo-server

bin/tcp-non-blocking-echo-server: tcp-non-blocking-echo-server.c
	cc tcp-non-blocking-echo-server.c -o bin/tcp-non-blocking-echo-server

bin/tcp-echo-server-multithread: tcp-echo-server-multithread.c
	cc tcp-echo-server-multithread.c -o bin/tcp-echo-server-multithread

clean:
	rm bin/*
