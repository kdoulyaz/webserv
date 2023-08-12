#include "../../include/webserv.hpp"


void Webserv::setuping()
{
	int	ret;
	const int enb = WA7ED;

	std::cout << "Getting address info..." << std::endl;
	if ((ret = getaddrinfo(host.c_str(), port.c_str(), &hints, &records))){
		std::cerr << gai_strerror(ret) << std::endl;
		exit(1);
	}
	std::cout << "Creating Socket..." << std::endl;
	sock_fd = socket(records->ai_family, records->ai_socktype, records->ai_protocol);
	fcntl(sock_fd, F_SETFL, O_NONBLOCK);
	maxfd_sock = sock_fd;
	if (sock_fd == -1){
		std::cerr << "socket: " << strerror(errno) << std::endl;
		exit(1);
	}
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enb, sizeof(int)) < 0){
		std::cerr << "setsockopt: " << strerror(errno) << std::endl;
		exit(1);
	}

	std::cout << "Binding socket to local address..." << std::endl;
	bind(sock_fd, records->ai_addr, records->ai_addrlen);
	//if bind fail
	freeaddrinfo(records);

	std::cout << "listening..." << std::endl;
	listen(sock_fd, BACK_LOG);
	// if listen fails
  	FD_SET(sock_fd, &net_fd);
}

void Webserv::init_fdbit()
{
  FD_ZERO(&this->fdread);
  FD_SET(this->sock_fd, &this->fdread);
  FD_ZERO(&this->fdwrite);
  FD_SET(this->sock_fd, &this->fdwrite);
	for (size_t i = 0; i < this->nets.size(); i++)
	{
		FD_SET(this->nets[i]->get_socket_fd(), &this->fdread);
    	FD_SET(this->nets[i]->get_socket_fd(), &this->fdwrite);
	}
}

void Webserv::multiplexing(Network *net, struct timeval &t)
{
// Response respons;
	init_fdbit();
	if (select(maxfd_sock + WA7ED, &fdread, &fdwrite, NULL, &t) < ZERO){
		std::cerr << "select: " << strerror(errno) << std::endl;
		exit(1);
	}
	for (int i = 3; i < maxfd_sock + WA7ED; i++){

		if (i != sock_fd and !(net = get_network(i)))
			continue;

		if (FD_ISSET(i, &fdread)){ // reading data from clients when FD_ISSET(i, &fdread_copy) is true
			if (i == sock_fd)
				add_network();
			else if (net and !net->is_readed){
				std::cout << "Receiving..." << std::endl;
				char buff[BUFFER_SIZE];
				int bytes = recv(net->get_socket_fd(), buff, sizeof(buff), 0);
				if (bytes < WA7ED)
					net->is_readed = true;
				else
					net->handle_req(buff, bytes);
			}
		}
		else if (FD_ISSET(i, &fdwrite)){
			// respons:  respons.handle_response(net);
			std::cout << "end" << std::endl;
		}
			// Client disconnected
	}	
		delete_network(net);
}