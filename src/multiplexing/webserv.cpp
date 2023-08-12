#include "header.hpp"

Webserv::Webserv()
: maxfd()
{
	FD_ZERO(&fdread);
	FD_ZERO(&fdwrite);
	FD_ZERO(&fderror);
}

const Network * Webserv::get_network(const int &s)
{
	std::vector<Network>::iterator it = nets.begin();
	while (it != nets.end())
	{
		if (it->sock_fd == s)
		{
			return it.base();
		}
		++it;
	}
	return NULL;
}

void Webserv::add_network(const bool &l, const int &s)
{
    nets.push_back(Network(l, s));
	if (s >= maxfd)
		maxfd = s + 1;
	FD_SET(s, &fdread);
	FD_SET(s, &fderror);
}

void Webserv::delete_network(const int &s)
{

  if ((net && net->is_done) == true)
  {
    std::cout << "Client disconnected." << std::endl;
    for (size_t i = 0; i < nets.size(); i++)
    {
      if (net->get_socket_fd() == nets[i]->get_socket_fd())
      {
        FD_CLR(net->get_socket_fd(), &net_fd);
        close(net->get_socket_fd());
        nets.erase(nets.begin() + i);
        delete net;
        break;
      }
    }
  }
}