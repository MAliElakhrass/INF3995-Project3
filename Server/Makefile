Server: Server.o
	g++ -g -o Server Server.o -l:libnavajo.a -l:libtag.a -lcrypto -lz -lpthread -lssl -l:libmad.a -lasound -lstdc++fs

Server.o: Server.cpp
	g++ -g -c Server.cpp

clean:
	rm Server Server.o
