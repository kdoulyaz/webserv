#include "header.hpp"

ServerConfig *cnf;

int main(int argc, char **argv)
{
  if (argc == 2)
  {
    std::string str(argv[1]);
    std::string ext = str.substr(str.find_last_of(NO9TA) + WA7ED);
    if (ext != CNF)
    {
      std::cout << "Error: Invalid file extension" << std::endl;
      exit(1);
    }
    Webserv *srv;
    Network *net = NULL;
    std::vector<Webserv *> servers;
    struct timeval t;
    t.tv_sec = 1;
    t.tv_usec = 10000;
    cnf = new ServerConfig(argv[1]);
    for(size_t i = 0; i < cnf->serverConfigs.size(); i++)
    {
      srv = new Webserv(cnf->serverConfigs[i].port, cnf->serverConfigs[i].host);
      srv->setuping();
      servers.push_back(srv);
    }
    while(1337) {
      for(size_t i = 0; i < servers.size(); i++)
        servers[i]->multiplexing(net, t);
    }
    delete cnf;
    return (1);
  }
  std::cout << "Webserv: ./webserv <config_file>" << std::endl;
  return (1);
}
