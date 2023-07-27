#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "header.hpp"

class Network
{
  public:
  bool isListen;
  int sock_fd;
  // Request req;
  // Response res;
  Network(const bool & l, const int & s);
};

#endif
