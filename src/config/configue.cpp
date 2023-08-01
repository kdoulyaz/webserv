#include "configue.hpp"

ServerConfig::ServerConfig()
{
}

ServerConfig::~ServerConfig()
{
}

void ServerConfig::parseServerConfigLine(std::string& line)
{
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ' '))
    {
        if (!token.empty())
            tokens.push_back(token);
    }

    if (tokens[0] == "host:")
        serverConfigs.back().host = tokens[1];
    else if (tokens[0] == "port:")
        serverConfigs.back().port = tokens[1];
    else if (tokens[0] == "max_body_size:")
        serverConfigs.back().maxBodySize = tokens[1];
    else if (tokens[0] == "server_name:"){
        for (size_t i = 1; i < tokens.size(); i++)
            serverConfigs.back().serverName.push_back(tokens[i]);
    }
    else if (tokens[0] == "error_page"){
        for (size_t i = 1; i < tokens.size(); i++)
        {
            if (tokens[i] == "400:")
                serverConfigs.back().errorPage400[tokens[i]] = tokens[i + 1];
            else if (tokens[i] == "404:")
                serverConfigs.back().errorPage404[tokens[i]] = tokens[i + 1];
        }
    }
    else if (tokens.size() == 3 && tokens[0] == "server_name:"){
        for (size_t i = 1; i < tokens.size(); i++)
            serverConfigs.back().serverName.push_back(tokens[i]);
    }
    else
        return ;
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
        for (size_t i = 1; i < tokens.size(); i++)
        {
            serverConfigs.back().locations.back().index.push_back(tokens[i]);
        }
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
    std::ifstream file(filename);

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
            else if (line.find("location /") != std::string::npos){
                serverConfigs.back().locations.push_back(LocationConfig());
                i =2;
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
