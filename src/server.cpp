#include "../include/server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

Server::Server(std::string &port, std::string &host)
{
  std::cout << "Server was Created" << std::endl;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  this->port = port;
  this->host = host;

  FD_ZERO(&fds);
}


void Server::get_address_info()
{
  int ret;

  std::cout << "Getting address info..." << std::endl;
  if ((ret = getaddrinfo(0, port.c_str(), &hints, &bind_address)))
  {
    std::cerr << gai_strerror(ret) << std::endl;
    exit(1);
  }
}

void Server::create_socket()
{
  const int enb = 1;
  std::cout << "Creating a Socket..." << std::endl;
  sock_fd = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
  if (sock_fd == -1)
  {
    std::cerr << "socket: " << strerror(errno) << std::endl;
    exit(1);
  }
  fcntl(sock_fd, F_SETFL, O_NONBLOCK);
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enb, sizeof(int)) < 0)
  {
    std::cerr << "setsockopt: " << strerror(errno) << std::endl;
    exit(1);
  }
}

void Server::bind_socket()
{
  std::cout << "Binding socket to local address..." << std::endl;
  if (bind(sock_fd, bind_address->ai_addr, bind_address->ai_addrlen))
  {
    std::cerr << "bind: " << strerror(errno) << std::endl;
    exit(1);
  }
  freeaddrinfo(bind_address);
}

void Server::listen_on_socket()
{
  std::cout << "listening..." << std::endl;
  if (listen(sock_fd, SOCKET_LISTEN))
  {
    std::cerr << "listen: " << strerror(errno) << std::endl;
    exit(1);
  }
}

void Server::create_server()
{
  get_address_info();
  create_socket();
  bind_socket();
  listen_on_socket();
  FD_SET(sock_fd, &fds);
}

void Server::clean_up()
{
  if (sock_fd != -1)
      close(sock_fd);
    if (bind_address)
      bind_address = nullptr;
    // Clear the file descriptor sets.
    FD_ZERO(&fds);
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
}