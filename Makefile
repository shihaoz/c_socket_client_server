
server_new: clean_server server
client_new: clean_client client
server:
	g++ server.cpp -o server
client:
	g++ client.cpp -o client

clean_server:
	rm server
clean_client:
	rm client