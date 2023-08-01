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
