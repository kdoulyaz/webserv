#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "header.hpp"

class ServerConfig
{
  private:
  public:
    void parseServerConfigLine(std::string& line);
    void parseLocationConfigLine(std::string& line);

    struct LocationConfig
    {
      std::string root;
      std::string autoindex;
      std::string uploadPath;
      std::string return_;
      std::vector<std::string> methods;
      std::vector<std::string> index;
      std::map<std::string, std::string> cgiPath;
      std::vector<std::string> locationConfig;
    };

    struct Server
    {
      std::string host;
      std::string port;
      std::string maxBodySize;
      std::map<std::string, std::string> errorPage400;
      std::map<std::string, std::string> errorPage404;
      std::vector<std::string> serverName;
      std::vector<LocationConfig> locations;
    };

    std::vector<Server> serverConfigs;

    ServerConfig();
    ~ServerConfig();
    ServerConfig(std::string filename);
};

#endif
