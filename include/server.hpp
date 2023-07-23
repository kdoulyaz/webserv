#ifndef SERVER_HPP
#define SERVER_HPP

#include "main.hpp"

class Server
{
private:
  struct addrinfo hints;
  struct addrinfo *bind_address;
  fd_set fds;
  std::string port;
  std::string host;
  fd_set read_fds;
  fd_set write_fds;
public:
  int sock_fd;
  Server();
  ~Server();
  Server(std::string &port, std::string &host);
  void get_address_info();
  void create_socket();
  void bind_socket();
  void listen_on_socket();
  void create_server();
  void clean_up();
};

#endif