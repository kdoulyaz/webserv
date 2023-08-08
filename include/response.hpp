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
        /* data */
    public:
        Response();
        ~Response();
        // variables

};

#endif