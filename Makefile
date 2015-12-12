all: serverSequential serverBounded serverUnbounded serverThreaded clientSocket launchClient

launchClient: launchClient.c
	gcc -o launchClient launchClient.c

serverBounded: serverBounded.c socketMng.o
	gcc -o serverBounded -g serverBounded.c socketMng.o

serverUnbounded: serverUnbounded.c socketMng.o
	gcc -o serverUnbounded -g serverUnbounded.c socketMng.o

serverSequential: serverSequential.c socketMng.o
	gcc -o serverSequential -g serverSequential.c socketMng.o

serverThreaded: serverThreaded.c socketMng.o
	gcc -pthread -o serverThreaded -g serverThreaded.c socketMng.o

clientSocket: clientSocket.c socketMng.o
	gcc -o clientSocket -g clientSocket.c socketMng.o

socketMng.o: socketMng.c
	gcc -c -g socketMng.c

clean: 
	rm -f *.o serverSequential serverBounded serverUnbounded serverThreaded clientSocket launchClient
