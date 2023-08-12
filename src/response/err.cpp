#include "../../include/response.hpp"

void Response::send_err(int net_socket, int code)
{
    std::map<int, std::string> response_map;
    // there are 399 status codes in the response
    // 100-199 are informational responses
    // 200-299 are success codes
    // 300-399 are redirection codes
    // 400-500 are client error codes
    // 501-599 are server error codes
    response_map[200] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 6\r\n\r\n200 OK";
    response_map[201] = "HTTP/1.1 201 Created\r\nContent-Type: text/html\r\nContent-Length: 7\r\n\r\nCreated";
    response_map[301] = "HTTP/1.1 301 Moved Permanently\r\nContent-Type: text/html\r\nContent-Length: 21\r\n\r\n301 Moved Permanently";
    response_map[400] = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: 15\r\n\r\n400 Bad Request";
    response_map[403] = "HTTP/1.1 403 Forbidden\r\nContent-Type: text/html\r\nContent-Length: 9\r\n\r\nForbidden";
    response_map[404] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 13\r\n\r\n404 Not Found";
    response_map[405] = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html\r\nContent-Length: 22\r\n\r\n405 Method Not Allowed";
    response_map[409] = "HTTP/1.1 409 Conflict\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\n409 Conflict";
    response_map[500] = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: 25\r\n\r\n500 Internal Server Error";
    response_map[501] = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\nContent-Length: 19\r\n\r\n501 Not Implemented";
    response_map[505] = "HTTP/1.1 505 HTTP Version Not Supported\r\nContent-Type: text/html\r\nContent-Length: 26\r\n\r\nHTTP Version Not Supported";
    response_map[413] = "HTTP/1.1 413 Pyload too large\r\nContent-Type: text/html\r\nContent-Length: 20\r\n\r\n413 Pyload too large";

    std::map<int, std::string>::iterator it = response_map.find(code);
    if (it == response_map.end()) // if the code is not in the map
    {
        std::cerr << "Error code not valid" << code << std::endl;
        return;
    }

    std::string response = it->second; // get the response from the map using the code as key
    send(net_socket, response.c_str(), response.length(), 0);
    // if send fails
}

int Response::check_serv(Network *net)
{
    Response res;
    std::string host = net->request.get_headers()[HOST];
    if (host.empty())
    {
        res.send_err(net->get_socket_fd(), 400);
        net->is_done = true;
        return 0;
    }
    std::string client_host = host.substr(0, host.find(":"));
    std::string client_port = host.substr(host.find(":") + WA7ED, host.size());
    for (size_t i = 0; i < cnf->serverConfigs.size(); i++)
    {
        for (size_t j = 0; j < cnf->serverConfigs[i].serverName.size(); j++)
        {
            if (cnf->serverConfigs[i].serverName[j] == client_host || cnf->serverConfigs[i].host == client_host)
                if ((cnf->serverConfigs[i].port == client_port))
                    return i;
        }
    }
    return -1;
}

int Response::loc_matched(Network *net, int index)
{
    Response res;
    std::string requeste = net->request.get_loc();

    for (size_t i = 0; i < cnf->serverConfigs[index].locations.size(); i++)
    {
        int j = 0;
        if (requeste  == cnf->serverConfigs[index].locations[i].locationConfig[j])
        {
            net->url = cnf->serverConfigs[index].locations[i].root + net->save_delete;
            return (i);
        }
        else
        {
            if (requeste[0] == 47 && requeste.find_last_of(SLASH) != 0) 
            {
                net->save_delete = requeste.substr(requeste.find_last_of(SLASH), requeste.size());
                requeste.erase(requeste.find_last_of(SLASH), requeste.size());
            }
            if (requeste  == cnf->serverConfigs[index].locations[i].locationConfig[j])
            {
                net->url = cnf->serverConfigs[index].locations[i].root + net->save_delete;
                return (i);
            }
        }
    }
    return (-1);
}

bool check_dir(std::string location)
{
    // check if file open if yes is directory or not
    if (open(location.c_str(), O_DIRECTORY) != -1) // O_DIRECTORY is a flag for directories not being opened properly for reading and writing
        return (true);
    else
        return (false);
}

bool is_allowed(std::string method, std::vector<std::string> &methods)
{
    if (std::find(methods.begin(), methods.end(), method) == methods.end())
        return (false);
    return (true);
}

int Response::post_err(Network *net)
{
    Response res;
    std::string ext;
    int index = check_serv(net);
        int is_match =  res.loc_matched(net, index);
    if (index == -1)
        index = 0;
    net->request.srv_index = index;
    net->request.location_index = is_match;
    ext = net->request.get_loc().substr(net->request.get_loc().find_last_of(NO9TA) + WA7ED);
    if (cnf->serverConfigs[index].locations[is_match].uploadPath.empty())
    {
        res.send_res(net, "404:");
        net->is_done = true;
        return 0;
    }
    if (!check_dir(cnf->serverConfigs[index].locations[is_match].uploadPath))
    {
        res.send_res(net, "404:");
        net->is_done = true;
        return 0;
    }
    if (!is_allowed(net->request.get_met(), cnf->serverConfigs[index].locations[is_match].methods))
    {
        res.send_err(net->get_socket_fd(), 405); // not allowed
        net->is_done = true;
        return 0;
    }
    if (cnf->serverConfigs[index].locations[is_match].cgiPath[ext] != WALO)
    {
        net->_cgi_path = net->request.get_loc();
        // res.cgi_res(net);
        net->is_done = true;
        return (1);
    }
    net->valid = true;
    return (1);
}
std::string toLowerCase(const std::string& str) {
    std::string result = str;
    for (std::string::iterator it = result.begin(); it != result.end(); ++it) {
        *it = tolower(*it);
    }
    return result;
}
void Network::handle_err() {
    size_t size;
    std::map<std::string, std::string>::iterator it = request.headers.find("Content-Type");
    std::map<std::string, std::string>::iterator it1 = request.headers.find("Content-Length");
    std::map<std::string, std::string>::iterator it2 = request.headers.find("Transfer-Encoding");
    std::istringstream iss(cnf->serverConfigs[request.srv_index].maxBodySize);
    iss >> std::hex >> request.max_body_size;
    if (iss.fail())
        std::cerr << "Error: body size" << std::endl;
    if (it == request.headers.end())
        request.is_err = 400;
    else if (it1 == request.headers.end() && it2 == request.headers.end())
        request.is_err = 400;
    else if (it1 != request.headers.end() && it2 != request.headers.end())
        request.is_err = 400;
    else if (it2 != request.headers.end() && toLowerCase(request.headers["Transfer-Encoding"]) != "chunked")
        request.is_err = 400;
    else if (it2 == request.headers.end() && it1 != request.headers.end()) {
    std::istringstream irr(request.headers["Content-Length"]);
    irr >> std::hex >> size;
    if (irr.fail() || size > request.max_body_size)
        request.is_err = 413;
    }
}