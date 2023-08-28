#include "header.hpp"

Webserv::Webserv() {}

Webserv::Webserv(std::string &port, std::string &host)
{
  std::cout << "Server was Created" << std::endl;

  bzero(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  this->port = port;
  this->host = host;

  FD_ZERO(&net_fd);
}

Network *Webserv::get_network(int f)
{
  std::vector<Network *>::iterator s = nets.begin();
  while (s < nets.end())
  {
    if ((*s)->get_socket_fd() == f)
    {
      return (*s);
    }
    s++;
  }
  return NULL;
}

void Webserv::add_network()
{
  Network *net = new Network();
  struct sockaddr_storage add;
  socklen_t addr_len = sizeof(add);
  int net_sock = accept(sock_fd, (struct sockaddr *)&add, &addr_len);
  fcntl(net_sock, F_SETFL, O_NONBLOCK);
  if (net_sock < 0)
  {
    std::cerr << "accept: " << strerror(errno) << std::endl;
    exit(1);
  }
  FD_SET(net_sock, &net_fd);
  if (net_sock > maxfd_sock)
    maxfd_sock = net_sock;
  net->set_socket_fd(net_sock);
  net->set_address(add);
  nets.push_back(net);
}

void Webserv::delete_network(const int &s)
{
  int maxtmp = 0;
  std::vector<Network *>::iterator it = nets.begin();
  while (it != nets.end())
  {
    if ((*it)->get_socket_fd() != s && (*it)->get_socket_fd() > maxtmp)
      maxtmp = (*it)->get_socket_fd();
    if ((*it)->get_socket_fd() == s)
    {
      close(s);
      delete *it;
      it = nets.erase(it);
      FD_CLR(s, &fdread);
      FD_CLR(s, &fdwrite);
    }
    else
      ++it;
  }
  maxfd_sock = maxtmp;
}

void  Webserv::setIndx(int i)
{
  this->indx = i;
}

void Webserv::buildResponse(Network &net)
{
  Response &rsp_ = *net.get_respo();
  Request &req_ = *net.get_request();
  req_.srv_index = indx;
  req_.location_index = 0;
  ServerConfig::Server &srv_ = cnf->serverConfigs[req_.srv_index];
  // ServerConfig::LocationConfig &loc_ = srv_.locations[req_.location_index];

  if (req_.is_err || rsp_.BuildBody(req_))
    rsp_.SrvErrPages(srv_, rsp_.get_sCode(), rsp_.getStatusMsg().c_str());
  if (rsp_.get_Cgi())
    return;
  else if (rsp_.get_AutIndx_Flag())
  {
    std::cout << "Auto Index" << std::endl;
    if (rsp_.BuildHtmlIndx())
    {
      rsp_.set_sCode(500);
      rsp_.BuildErrPage(req_);
    }
    else
    {
      rsp_.set_sCode(200);
      rsp_.set_bdyTorespo();
    }
  }
  rsp_.set_StatusLin();
  rsp_.addFields(req_);
  if ((req_.get_met() == GET) || (rsp_.get_sCode() != 200))
    rsp_.payload_content.append(rsp_.get_respo_body());
}

void Webserv::sendRespo(Network &c)
{
  size_t byts_sent = 0;
  Response &rsp_ = *c.get_respo();
  std::string response = rsp_.get_Content();
  std::cerr << "{" << response.length() << "}" << byts_sent <<std::endl;
  if (response.length() >= MSG_BUFF && response.length() > 0)
    byts_sent = write(c.get_socket_fd(), response.c_str(), MSG_BUFF);
  else
    byts_sent = write(c.get_socket_fd(), response.c_str(), response.length());
  if (byts_sent < 0)
    std::cerr << "here coldnt wrtie to server" << std::endl;
  else if (byts_sent == 0 || byts_sent == response.length())
  {
    if (c.get_request()->connection_status() == false || c.get_request()->is_err || c.get_respo()->get_Cgi())
    {
      delete_network(c.get_socket_fd());
    }
    else
    {
      rsp_.respo_Cut(byts_sent);
    }
  }
}

int _stoi(std::string str)
{
    std::stringstream ss(str);
    if (str.length() > 10)
        throw std::exception();
    for (size_t i = 0; i < str.length(); ++i)
    {
        if(!isdigit(str[i]))
            throw std::exception();
    }
    int res;
    ss >> res;
    return (res);
}

