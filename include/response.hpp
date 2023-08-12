#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include "request.hpp"
#include "config.hpp"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
class ServerConfig;
class Network;

class Response
{
    private:
 
        std::string target;
        std::vector<uint8_t> body;
        size_t body_len;
        std::string response;
        std::string location;
        short code;
        char *raw_res;
        int cgi;
        int cgi_fd[2];
        size_t cgi_res_len;
        bool auto_index;

    public:
        Response();
        ~Response();
        // variables

};

#endif