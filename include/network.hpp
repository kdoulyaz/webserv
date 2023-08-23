#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "header.hpp"

class Network
{
private:
  struct sockaddr_storage address;
  socklen_t addr_size;
  int socket_fd;
  int rcved;
  bool header;
  Request request;
  Response  _respo;

public:
  Network();
  ~Network();
  void      set_socket_fd(int fd);
  void      set_address(struct sockaddr_storage &address);
  int       get_socket_fd();
  Response  *get_respo();
  Request   *get_request();
  void      handle_req(const char *req_body, size_t length);
  bool      handle_post(Network *c);
  bool      is_read;
  bool      is_done;
  
};

#endif
