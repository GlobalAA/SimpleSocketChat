#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define ERROR_S "SERVER ERROR: "
#define PORT 5553
#define BUFFER_SIZE 1024

bool is_client_connection_close(const char* msg) {
	for(int i = 0; i < strlen(msg); i++) {
		if (msg[i] == '#') {
			return true;
		}
	}
	return false;
}

int main() {
	int client;
	int server;
	
	struct sockaddr_in server_address;

	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0) {
		std::cout << ERROR_S << "establishing socket error";
		return -1;
	}

	std::cout << "SERVER: Socket for server was successfully created\n";

	server_address.sin_port = htons(PORT);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htons(INADDR_ANY);

	int ret = bind(client, (struct sockaddr*)&server_address, sizeof(server_address));

	if (ret < 0) {
		std::cout << ERROR_S << "binding connection. Socket has already been establishing\n";
		return -1;
	}

	socklen_t size = sizeof(server_address);
	std::cout << "SERVER: " << "Listening clients...\n";

	listen(client, 1);
	server = accept(client, (struct sockaddr*)(&server_address), &size);
	if (server < 0) {
		std::cout << ERROR_S << "Can`t accepting client\n";
	}

	char buffer[BUFFER_SIZE];
	bool isExit = false;
	while (server > 0) {
		strcpy(buffer, "=> Server connected!\n");
		send(server, buffer, BUFFER_SIZE, 0);
		std::cout << "=> Connected to the client #1" << std::endl
			<< "Enter # to end the connection\n\n";
			
		std::cout << "Client: ";
		recv(server, buffer, BUFFER_SIZE, 0);
		std::cout << buffer << std::endl;
		if (is_client_connection_close(buffer)) {
			isExit = true;
		}

		while (!isExit) {
			std::cout << "Server: ";
			std::cin.getline(buffer, BUFFER_SIZE);
			send(server, buffer, BUFFER_SIZE, 0);
			if (is_client_connection_close(buffer)) {
				break;
			}

			std::cout << "Client: ";
			recv(server, buffer, BUFFER_SIZE, 0);
			std::cout << buffer << std::endl;
			if (is_client_connection_close(buffer)) {
				break;
			}
		}
		isExit = false;
		close(server);
		std::cout << "\nGoodBye..." << std::endl;
		exit(0);
	}
}