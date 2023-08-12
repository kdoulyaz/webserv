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
    int maxtmp = 0;
    std::vector<Network*>::iterator it = nets.begin(); 
    while (it != nets.end())
    {
        if ((*it)->get_socket_fd() != s && (*it)->get_socket_fd() > maxtmp)
            maxtmp = (*it)->get_socket_fd();
        if ((*it)->get_socket_fd() == s)
        {
            close(s);
            delete *it; 
            it = nets.erase(it);
            FD_CLR(s, &fdread);
            FD_CLR(s, &fdwrite);
        }
        else
            ++it;
    }
    maxfd_sock = maxtmp;
}