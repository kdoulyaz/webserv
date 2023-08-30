#include "../../include/header.hpp"

ServerConfig::ServerConfig()
{
    initErrPages();
}

ServerConfig::~ServerConfig()
{
}

/* init error page by default */
void ServerConfig::initErrPages()
{
    // Access the first server's errorPage map
    if (!serverConfigs.empty() && !serverConfigs[0].errorPage.empty())
    {
        // Use a reference to the errorPage map for convenience
        std::map<short, std::string> &errorPage = serverConfigs[0].errorPage;

        errorPage[301] = "";
        errorPage[302] = "";
        errorPage[400] = "";
        errorPage[401] = "";
        errorPage[402] = "";
        errorPage[403] = "";
        errorPage[404] = "";
        errorPage[405] = "";
        errorPage[406] = "";
        errorPage[500] = "";
        errorPage[501] = "";
        errorPage[502] = "";
        errorPage[503] = "";
        errorPage[505] = "";
    }
}

void ServerConfig::parseServerConfigLine(std::string& line)
{
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    char *end = 0;

    while (std::getline(ss, token, ' '))
    {
        if (!token.empty())
            tokens.push_back(token);
    }

    if (tokens[0] == "host:")
        serverConfigs.back().host = tokens[1];
    else if (tokens[0] == "port:")
        serverConfigs.back().port = tokens[1];
    else if (tokens[0] == "root")
        serverConfigs.back().super_root = tokens[1];
    else if (tokens[0] == "index")
        serverConfigs.back().srv_index = tokens[1];
    else if (tokens[0] == "max_body_size:" && token[1])
    {
        serverConfigs.back().maxBodySize = strtoul(tokens[1].c_str(),&end, 10);
        if (end != '\0'){
            return ; //should handel errros  in  this file 
        }
    }
    else if (tokens[0] == "server_name:")
    {
        for (size_t i = 1; i < tokens.size(); i++)
            serverConfigs.back().serverName.push_back(tokens[i]);
    }
    else if (tokens[0] == "error_page")
    {
        for (size_t i = 1; i < tokens.size() - 1; i += 2)
        {
            short errorCode;
            std::stringstream(tokens[i]) >> errorCode;
            serverConfigs.back().errorPage[errorCode] = tokens[i + 1];
        }
    }
    else if (tokens.size() == 3 && tokens[0] == "server_name:")
    {
        for (size_t i = 1; i < tokens.size(); i++)
            serverConfigs.back().serverName.push_back(tokens[i]);
    }
    else
        return;
}


void ServerConfig::parseLocationConfigLine(std::string& line)
{
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ' '))
    {
        if (!token.empty())
            tokens.push_back(token);
    }

    if (tokens.empty() || tokens[0] == "}")
        return;
    else if (tokens[0] == "root:")
        serverConfigs.back().locations.back().root = tokens[1];
    else if (tokens[0] == "index:")
    {
        serverConfigs.back().locations.back().loc_index = tokens[1];
    }
    else if (tokens[0] == "autoindex:")
        serverConfigs.back().locations.back().autoindex = tokens[1];
    else if (tokens.size() == 3 && tokens[0] == "cgi_path:")
        serverConfigs.back().locations.back().cgiPath[tokens[1]] = tokens[2];
    else if (tokens[0] == "methods:")
    {
        for (size_t i = 1; i < tokens.size(); i++)
        {
            serverConfigs.back().locations.back().methods.push_back(tokens[i]);
        }
    }
    else if (tokens[0] == "return:")
        serverConfigs.back().locations.back().return_ = tokens[1];
    else if (tokens[0] == "upload_path:")
        serverConfigs.back().locations.back().uploadPath = tokens[1];
    else if (tokens.size() == 3 && tokens[0] == "cgi_path:")
        serverConfigs.back().locations.back().cgiPath[tokens[1]] = tokens[2];
    else
        return ;
}

ServerConfig::ServerConfig(std::string filename)
{
    std::ifstream file(filename.c_str());

    if (file.is_open())
    {
        std::string line;
        int         i;

        while (std::getline(file, line))
        {
            if (line.empty())
                continue;
            if (line.find("server {") != std::string::npos){
                serverConfigs.push_back(Server());
                i =1;
                continue;
            }
            else if (line.find("location ") != std::string::npos)
            {
                serverConfigs.back().locations.push_back(LocationConfig());
                size_t startPos = line.find("location ") + strlen("location ");
                std::string parsedWord;

                for (size_t i = startPos; i < line.length(); ++i)
                {
                    if (line[i] == ' ' || line[i] == '{')
                    {
                        break;
                    }
                    parsedWord += line[i];
                }
                serverConfigs.back().locations.back().path_ = parsedWord;
                i = 2;
                continue;
            }
            if (serverConfigs.empty())
            {
                std::cout << "No server found" << std::endl;
                exit(1);
            }
            else if (i == 1)
                parseServerConfigLine(line);
            else if (i == 2)
                parseLocationConfigLine(line);
            else
                std::cout << "Error: " << line << std::endl;
        }
    }
    else
    {
        std::cout << "Unable to open file" << std::endl;
        exit(1);
    }

    file.close();
}

const std::vector<ServerConfig::LocationConfig>::iterator ServerConfig::Server::getLocationKey(std::string key)
{
    std::vector<LocationConfig>::iterator it;
    for (it = locations.begin(); it != locations.end(); it++)
    {
        if (it->getPath() == key)
            return it;
    }
    std::cerr << "Error: path to location not found[" << key << "]" << std::endl;
    return locations.end();
}

std::string ServerConfig::LocationConfig::getPath() const
{
    return path_;
}

std::string ServerConfig::LocationConfig::getLocIndx() const
{
    return loc_index;
}