#include "header.hpp"

void setuping(Webserv &webserv)
{
	struct addrinfo hints;
	struct addrinfo *records;
	for (size_t i = 0; i < webserv.config->serverConfigs.size(); i++)
	{
		std::cout << "Server Block number " << i + 1 << std::endl;
		const std::string &host = webserv.config->serverConfigs[i].host;
		const std::string &port = webserv.config->serverConfigs[i].port;
		int sock_fd;

		std::cout << "Getting address info..." << std::endl;
		bzero(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		getaddrinfo(host.c_str(), port.c_str(), &hints, &records);

		std::cout << "Creating Socket..." << std::endl;
		sock_fd = socket(records->ai_family, records->ai_socktype, records->ai_protocol);
		fcntl(sock_fd, F_SETFL, O_NONBLOCK);

		std::cout << "Binding socket to local address..." << std::endl;
		bind(sock_fd, records->ai_addr, records->ai_addrlen);
		freeaddrinfo(records);

		std::cout << "listening..." << std::endl;
		listen(sock_fd, BACK_LOG);

		std::cout << "Adding the Socket..." << std::endl;
		webserv.add_network(SLISTEN, sock_fd);
	}
}

void init_fdbit(Webserv &webserv, fd_set &fdread_copy, fd_set &fdwrite_copy, fd_set &fderror_copy)
{
	FD_ZERO(&fdread_copy);
	FD_ZERO(&fdwrite_copy);
	FD_ZERO(&fderror_copy);
	FD_COPY(&webserv.fdread, &fdread_copy);
	FD_COPY(&webserv.fdwrite, &fdwrite_copy);
	FD_COPY(&webserv.fderror, &fderror_copy);
}

void multiplexing(Webserv &webserv)
{
	struct timeval t;
	fd_set fdread_copy;
	fd_set fdwrite_copy;
	fd_set fderror_copy;
	std::vector<char> buff(2048);

	// int bytes = 0;

	t.tv_sec = 1;
	t.tv_usec = 10000;
	while (true)
	{
		init_fdbit(webserv, fdread_copy, fdwrite_copy, fderror_copy);
		select(webserv.maxfd, &fdread_copy, &fdwrite_copy, &fderror_copy, &t);
		for (int fd_sock = 3; fd_sock < webserv.maxfd; ++fd_sock)
		{
			if (FD_ISSET(fd_sock, &fdread_copy)) // reading data from clients when FD_ISSET(fd_sock, &fdread_copy) is true
			{
				if (webserv.get_network(fd_sock)->isListen == SLISTEN)
				{
					int client_sock = accept(fd_sock, NULL, NULL);
					std::cout << "Say Welcome to the new Client" << std::endl;
					webserv.add_network(!SLISTEN, client_sock);
				}
				else
				{
					std::cout << "Receiving..." << std::endl;
					int bytes = recv(webserv.get_network(fd_sock)->sock_fd, buff.data(), buff.size(), 0);
					if (bytes > 0)
					{
						// Parseing
						for (int i = 0; i < bytes; i++)
						{
							std::cout << buff[i];
						}
						std::cout << std::endl;
					}
					else if (bytes == 0)
					{
						// Client disconnected
						std::cout << "Client disconnected." << std::endl;
						webserv.delete_network(fd_sock);
					}
					else
					{
						// Handle recv error
						std::cerr << "Error receiving data from client." << std::endl;
					}
				}
			}
			else if (FD_ISSET(fd_sock, &fdwrite_copy)) // sending data to clients when FD_ISSET(fd_sock, &fdwrite_copy) is true
			{
				std::cout << "Sending..." << std::endl;
			}
			else if (FD_ISSET(fd_sock, &fderror_copy))
			{
				std::cout << "Deleting..." << std::endl;
				webserv.delete_network(fd_sock);
			}
		}
	}
}
