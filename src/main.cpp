#include "header.hpp"


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
