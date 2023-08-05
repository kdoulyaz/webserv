#ifndef webserv_hpp
#define webserv_hpp

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
  const Network * get_network(const int &s);
  void rtow(const int &s); // convert  from read   to write 
  void add_network(const bool &l, const int &s);
  void delete_network(const int &s);
};

#endif
