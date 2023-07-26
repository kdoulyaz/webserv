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

void Webserv::from_read_to_write(const int &s)
{
	FD_CLR(s, &fdread);
	FD_SET(s, &fdwrite);
}


void Webserv::delete_network(const int &s)
{
	int maxtmp;
	std::vector<Network>::iterator it = nets.begin();
	while (it != nets.end())
	{
		if (it->sock_fd > maxtmp)
			maxtmp = it->sock_fd;
		if (it->sock_fd == s)
		{
			close(s);
			nets.erase(it);
			FD_CLR(s, &fdread);
			FD_CLR(s, &fdwrite);
		}
		++it;
	}
	maxfd = maxtmp + 1;
}
