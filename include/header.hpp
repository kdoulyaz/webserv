#ifndef HEADER_HPP
#define HEADER_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <cstring>

class ServerConfig;
class Network;
class Webserv;

#include "webserv.hpp"
#include "configue.hpp"
#include "network.hpp"

#define SLISTEN true

#define PORT "8011"
#define BACK_LOG 1024
#define BUFFER_SIZE 4096
#define CRLF "\r\n"

#endif
