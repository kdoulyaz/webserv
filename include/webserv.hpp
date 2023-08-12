#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "header.hpp"

class Webserv
{
  public:
  ServerConfig *config;
  std::vector<Network> nets;
  int maxfd;
  fd_set fdread;
  fd_set fdwrite;
  fd_set fderror;

    Webserv();
    Webserv(std::string &port, std::string &host);
    void  setuping();
    Network *get_network(int s);
    void  add_network();
    void  delete_network(const int &s);
    void  multiplexing(Network *net, struct timeval &t);
    void  init_fdbit();
};

#endif
