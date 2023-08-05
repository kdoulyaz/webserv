#ifndef CGI_HPP
#define CGI_HPP

#include <cstddef>
#include <iostream>
#include <map>
#include <sched.h>
#include <string>
#include <unistd.h>

class requestTmp {
public:
  // GET /form.php HTTP:/1.1
  std::string requestLine[3];
  // script name file (php or py)
  std::string filePath;
  // if the method is post the name of posted file
  std::string postFile;
  std::string query;
  std::map<std::string, std::string> headers;
  // get it from request
  requestTmp() {
    requestLine[0] = "Get";
    requestLine[1] = "/form.php";
    requestLine[2] = "http:/1.1";
    filePath = "./form.php";
    filePath = "post.png";
    query = " name=value&param=example";
    {
      headers["Host"] = "example.com";
      headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; "
                              "rv:90.0) Gecko/20100101 Firefox/90.0";
      headers["Content-Type"] = "image/png";
      headers["Content-Length"] = "34";
      headers["Authorization"] = "Bearer your_access_token";
    }
  }
};

class Cgi {

public:
  char **Envp;
  pid_t pid;
  int statusCode;
  int input;
  int fd_out;
  Cgi();
  ~Cgi();
  void initEnv(requestTmp &);
  void exeCgi(requestTmp &request, std::string);
};

#endif
