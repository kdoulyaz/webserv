#include "header.hpp"

Network::Network()
{
  addr_size = sizeof(address);
  rcved = 0;
  header = true;
  is_read = false;
}

Network::~Network()
{}

int Network::get_socket_fd()
{
  return socket_fd;
}

void Network::set_socket_fd(int fd)
{
  socket_fd = fd;
}

void Network::set_address(struct sockaddr_storage &address)
{
  this->address = address;
}


bool Network::handle_post(Network *c)
{
  (void)* c;
  //respons
  return(1);
}

void Network::handle_req(const char *req_body, size_t size)
{
  std::string ext;
  std::string str_header;
  std::string body;
  std::string url;
  std::string str(req_body, size);
  int pos;
  if (header)
  {
    pos = str.find(SEP);
    str_header = str.substr(0, pos);
    body = str.substr(pos + 4);
    request.handle_headers(str_header);
    url = request.get_loc();
    ext = url.substr(url.find_last_of(NO9TA) + WA7ED);
    if (request.get_met() == "POST" and handle_post(this) and cnf->serverConfigs[request.srv_index].locations[request.location_index].cgiPath[ext].empty())
    {

      if (!request.is_err)
        request.handle_body(body);
      header = false;
    }
        std::cout << "end" << std::endl;
  }
  else if (request.get_met() == "POST" and handle_post(this) and !request.is_err and cnf->serverConfigs[request.srv_index].locations[request.location_index].cgiPath[ext].empty())
    request.handle_body(str);
}