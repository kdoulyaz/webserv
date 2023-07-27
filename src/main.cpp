#include "header.hpp"

// void printConfiguration()
// {
//     for (size_t i = 0; i < config->serverConfigs.size(); i++)
//     {
//         std::cout << "Server host: " << config->serverConfigs[i].host << std::endl;
//         std::cout << "Server port: " << config->serverConfigs[i].port << std::endl;
//         std::cout << "Max body size: " << config->serverConfigs[i].maxBodySize << std::endl;
//         std::cout << "Error page 400: " << config->serverConfigs[i].errorPage400["400:"] << std::endl;
//         std::cout << "Error page 404: " << config->serverConfigs[i].errorPage404["404:"] << std::endl;

//         for (size_t k = 0; k < config->serverConfigs[i].serverName.size(); k++)
//             std::cout << "Server name: " << config->serverConfigs[i].serverName[k] << std::endl;

//         for (size_t j = 0; j < config->serverConfigs[i].locations.size(); j++)
//         {
//             std::cout << "Location /" << j << std::endl;
//             std::cout << "Methods: ";
//             
//             for (size_t k = 0; k < config->serverConfigs[i].locations[j].methods.size(); k++)
//                 std::cout << config->serverConfigs[i].locations[j].methods[k] << " ";
//             
//             std::cout << std::endl;
//             std::cout << "Root: " << config->serverConfigs[i].locations[j].root << std::endl;
//             
//             std::cout << "Index: ";
//             for (size_t k = 0; k < config->serverConfigs[i].locations[j].index.size(); k++)
//                 std::cout << config->serverConfigs[i].locations[j].index[k] << " ";
//             
//             std::cout << std::endl;
//             std::cout << "Upload path: " << config->serverConfigs[i].locations[j].uploadPath << std::endl;
//             std::cout << "Autoindex: " << config->serverConfigs[i].locations[j].autoindex << std::endl;

//             std::cout << "CGI Paths:" << std::endl;
//             for (std::map<std::string, std::string>::const_iterator it = config->serverConfigs[i].locations[j].cgiPath.begin();
//                  it != config->serverConfigs[i].locations[j].cgiPath.end(); ++it)
//             {
//                 const std::string& key = it->first;
//                 const std::string& value = it->second;
//                 std::cout << "  " << key << ": " << value << std::endl;
//             }
//             std::cout << std::endl;
//         }
//     }
// }

void setuping(Webserv & webserv)
{
  struct addrinfo hints;
  struct addrinfo *records;
  for(size_t i = 0; i < webserv.config->serverConfigs.size(); i++)
  {
    std::cout << "Server Block number " << i + 1 << std::endl;
    const std::string & host = webserv.config->serverConfigs[i].host;
    const std::string & port = webserv.config->serverConfigs[i].port;
    int sock_fd;

    std::cout << "Getting address info..." << std::endl;
    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    getaddrinfo(host.c_str(), port.c_str(), &hints, &records);

    std::cout << "Creating Socket..." << std::endl;
    sock_fd = socket(records->ai_family, records->ai_socktype, records->ai_protocol);
    fcntl(sock_fd, F_SETFL, O_NONBLOCK);

    std::cout << "Binding socket to local address..." << std::endl;
    bind(sock_fd, records->ai_addr, records->ai_addrlen);
    freeaddrinfo(records);

    std::cout << "listening..." << std::endl;
    listen(sock_fd, BACK_LOG);

    std::cout << "Adding the Socket..." << std::endl;
	webserv.add_network(SLISTEN, sock_fd);
  }
}

void multiplexing(Webserv & webserv)
{
  struct timeval t;
  t.tv_sec = 1;
  t.tv_usec = 10000;
  fd_set fdread_copy;
  fd_set fdwrite_copy;
  fd_set fderror_copy;

  while (true)
  {
	FD_ZERO(&fdread_copy);
	FD_ZERO(&fdwrite_copy);
	FD_ZERO(&fderror_copy);
	FD_COPY(&webserv.fdread, &fdread_copy);
	FD_COPY(&webserv.fdwrite, &fdwrite_copy);
	FD_COPY(&webserv.fderror, &fderror_copy);

	select(webserv.maxfd, &fdread_copy, &fdwrite_copy, &fderror_copy, &t);

	for (int fd_sock = 3; fd_sock < webserv.maxfd; ++fd_sock)
	{
		if (FD_ISSET(fd_sock, &fdread_copy))
		{
			if (webserv.get_network(fd_sock)->isListen == SLISTEN)
			{
				int client_sock = accept(fd_sock, NULL, NULL);
				std::cout << "Say Welcome to the new Client" << std::endl;
				webserv.add_network(!SLISTEN, client_sock);
			}
			else
				std::cout << "Receiving..." << std::endl;
		}
		else if (FD_ISSET(fd_sock, &fdwrite_copy))
		{
			std::cout << "Sending..." << std::endl;
		}
		else if (FD_ISSET(fd_sock, &fderror_copy))
		{
			std::cout << "Deleting..." << std::endl;
			webserv.delete_network(fd_sock);
		}
	}
  }
}

int main(int argc, char **argv)
{
  Webserv webserv;

  if (argc != 2)
  {
    std::cout << "Usage: ./webserv <config_file>" << std::endl;
    return 1;
  }
  webserv.config = new ServerConfig(argv[1]);
  setuping(webserv);
  multiplexing(webserv);
  std::cout << "All is good !" << std::endl;
  return 0;
}
