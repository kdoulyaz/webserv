#include "header.hpp"

Network::Network(const bool & l, const int & s)
: isListen(l)
, sock_fd(s)
{}
