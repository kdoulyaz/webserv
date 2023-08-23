
#ifndef CONFIGUE_HPP
#define CONFIGUE_HPP
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
      std::string getPath() const;
    };

    struct Server
    {
      std::string host;
      std::string port;
      std::string maxBodySize;
      std::map<short, std::string> errorPage;
    //  std::map<std::string, std::string> errorPage404;
      std::vector<std::string> serverName;
      std::vector<LocationConfig> locations;
      const std::vector<LocationConfig>::iterator getLocationKey(std::string key);
    };

    std::vector<Server> serverConfigs;

    ServerConfig();
    ~ServerConfig();
    ServerConfig(std::string filename);
    void  initErrPages(void);
};
#endif

