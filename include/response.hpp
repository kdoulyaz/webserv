#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "header.hpp"
#include "configue.hpp"

class ServerConfig;
class Network;

class Response
{
    private:
        /* data */
    public:
        Response();
        ~Response();
        int post_err(Network *net);
        int check_serv(Network *net);
        void send_err(int net_socket, int code);
        int loc_matched(Network *net, int index);
        void send_res(Network *net, std::string key);
        std::string getContentType(std::string &filename);
        void  deleteMethod(std::string &fileName);



};

#endif
