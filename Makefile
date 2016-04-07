all_new: clean_obj server_new client_new

server_new: clean_server server

client_new: clean_client client

server: socket_tcp.o
	g++ *.o server.cpp -o server

client: socket_tcp.o
	g++ *.o client.cpp -o client

socket_tcp.o: socket_tcp.hpp socket_tcp.cpp
	g++ socket_tcp.cpp -c 

clean_server:
	rm server

clean_client:
	rm client

clean_obj:
	rm *.o