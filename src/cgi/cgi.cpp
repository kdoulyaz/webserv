#include "./cgi.hpp"
#include <bits/types/FILE.h>
#include <cstddef>
#include <cstring>
#include <map>
#include <sched.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define FILE_PATH "none"
#define GUERY "none"
#define ADDR "none"

Cgi::Cgi() { std::cout << "constructer called " << std::endl; }

Cgi::~Cgi() { std::cout << "destructer called" << std::endl; }

void Cgi::initEnv(requestTmp &request) {
  int index = 0;
  std::string tmp;
  std::string contentLenghtValue;
  std::string contentTypeValue;
  int find;
  typedef std::map<std::string, std::string>::iterator iterator;
  for (iterator IT = request.headers.begin(); IT != request.headers.end();
       IT++) {
    if (IT->first == "content-lenght") {
      (contentLenghtValue = IT->second);
      continue;
    } else if (IT->first == "content-type") {
      contentTypeValue = IT->second;
      continue;
    }

    for (size_t index = 0; index < tmp.length(); ++index)
      tmp[index] = std::toupper(tmp[index]);
    tmp = "HTTP_" + IT->first + "=";
    find = tmp.find("-");
    if (find != std::string::npos)
      tmp[find] = '_';
    tmp.append(IT->second);
    Envp[index++] = strdup(tmp.c_str());
  };

  Envp[index++] = strdup(("CONTENTTYPE=" + contentTypeValue).c_str());
  Envp[index++] = strdup(("CONTENTLENGHT=" + contentLenghtValue).c_str());
  Envp[index++] = strdup(("REQUEST_METHOD=" + request.requestLine[0]).c_str());
  Envp[index++] = strdup("REDIRECT_STATUS=200");
  Envp[index++] = strdup(request.filePath.c_str());
  Envp[index++] = strdup(("SCRIPT_FILENAME=" + request.filePath).c_str());
  Envp[index++] = strdup(("QUERY_STRING=" + request.query).c_str());
  Envp[index++] = NULL;
}

void Cgi::exeCgi(requestTmp &request, std::string filePath) {

  statusCode = 0;

  pid = fork();
  if (!pid) {
    char **args = new char *[3];
    args[0] = strdup("add exe cgi");
    args[1] = strdup(filePath.c_str());
    args[2] = NULL;
    Envp = new char *[request.headers.size() + 8];
    int i = 0;
    initEnv(request);
    if (request.requestLine[0] == "POST") {
      dup2(input, STDIN_FILENO);
      close(input);
    }
    dup2(fd_out, STDOUT_FILENO);
    close(input);
    execve(args[0], args, Envp);
    exit(68);
  }
  if (request.requestLine[0] == "POST")
    close(input);
  close(fd_out);
  close(fd_out);
  waitpid(pid, &statusCode, 0);
}
