cc=g++

.PHONY:all
all:tcpClient tcpServer

tcpClient:tcpClient.cc
	$(cc) -o $@ $^ -std=c++11
tcpServer:tcpServer.cc
	# $(cc) -o $@ $^ -std=c++11 -DPROCESS1
	$(cc) -o $@ $^ -std=c++11 -DPROCESS2

.PHONY:clean
clean:
	rm -f tcpClient tcpServer