cc=g++

.PHONY:all
all:tcpClient tcpServer

tcpClient:tcpClient.cc
	$(cc) -o $@ $^ -std=c++11
tcpServer:tcpServer.cc
	$(cc) -o $@ $^ -std=c++11 -lpthread -DTHREAD_POOL

.PHONY:clean
clean:
	rm -f tcpClient tcpServer log.txt log.error