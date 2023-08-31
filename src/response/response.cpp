#include "header.hpp"

Response::Response()
{
    _target = "";
    body_data.empty();
    body_len = 0;
    payload_content = "";
    respo_body = "";
    location_header = "";
    _scode = 0;
    cgi_flag = 0;
    cgi_pipfd[0] = -1;
    cgi_pipfd[1] = -1;
    cgi_respo_len = 0;
    aut_indx_flag = false;
}

int Response::get_sCode() const
{
    return (_scode);
}

int Response::get_Cgi() const
{
    return (cgi_flag);
}

std::string Response::getStatusMsg() const
{
    return (statusMsg);
}

std::string    Response::get_Content()
{
    return (payload_content);
}

bool Response::get_AutIndx_Flag() const
{
    return (aut_indx_flag);
}

std::string Response::get_target() const
{
    return (_target);
}

size_t Response::get_body_len() const
{
    return (body_len);
}

std::string Response::get_respo_body() const
{
    return (respo_body);
}

void Response::setResponseHeader(const short &_code, const std::string &_msg)
{
    this->_scode = _code;
    this->statusMsg = _msg;
}

void Response::set_sCode(short scode)
{
    _scode = scode;
}

void Response::set_bdyTorespo()
{
    respo_body.insert(respo_body.begin(), body_data.begin(), body_data.end());
}

/* Constructs Status line based on status code. */
void Response::set_StatusLin()
{
    payload_content.append("HTTP/1.1 " + toString(_scode) + " ");
    payload_content.append(_sCodeStr(_scode));
    payload_content.append("\r\n");
}

Response::~Response()
{
}

void Response::addFiled_date()
{
    char d[1000];
    time_t now = time(0);
    struct tm tn = *gmtime(&now);

    strftime(d, sizeof(d), "%a, %d %b %Y %H:%M:%S %Z", &tn);
    payload_content.append("Date: ");
    payload_content.append(d);
    payload_content.append("\r\n");
}

void Response::addFiled_location()
{
    if (location_header.length())
        payload_content.append("Location: " + location_header + "\r\n");
}

void Response::addField_Server()
{
    payload_content.append("Server: SrVr\r\n");
}

void Response::addField_ConnectionFlag(Request &req_)
{
    if (req_.connection_status())
        payload_content.append("Connection: keep-alive\r\n");
}

void Response::addFiled_ContentLen()
{
    std::stringstream strstm;
    strstm << respo_body.length();
    payload_content.append("Content-Length: ");
    payload_content.append(strstm.str());
    payload_content.append("\r\n");
}

void Response::addField_ContentType()
{
    payload_content.append("Content-Type: ");
    if (_target.rfind(".", std::string::npos) != std::string::npos && _scode == 200)
        payload_content.append(contentype.getConteType(_target.substr(_target.rfind(".", std::string::npos))));
    else
        payload_content.append(contentype.getConteType("default"));
    payload_content.append("\r\n");
}

void Response::addFields(Request &req_)
{
    addField_ContentType();
    addFiled_ContentLen();
    addField_ConnectionFlag(req_);
    addField_Server();
    addFiled_location();
    addFiled_date();
    payload_content.append("\r\n");
}

void Response::SrvErrPages(const ServerConfig::Server &srv_, const short &status_, const char *statusMsg_)
{
    std::map<short, std::string>::const_iterator it =
        srv_.errorPage.find(status_);
    std::string tmp = std::string(statusMsg_);

    setResponseHeader(status_, tmp);
    if (it == srv_.errorPage.end())
    {
        respo_body = getErrPage(_scode);
    }
}

std::string Response::toString(short val) const
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

std::string Response::getErrPage(short err_code) const
{
    std::string errCodeStr = this->toString(err_code);

    return ("<html>\r\n<head><title>" + errCodeStr + " " +
            "Error" + " </title></head>\r\n" + "<body>\r\n" +
            "<center><h1>" + errCodeStr + " " + "Error" + "</h1></center>\r\n");
    // std::string p = "<!DOCTYPE html>\n"
    //                     "<html>\n"
    //                     "<head>\n"
    //                     "    <title>Error Page</title>\n"
    //                     "</head>\n"
    //                     "<body>\n"
    //                     "    <h1>Oops! Something went wrong.</h1>\n"
    //                     "    <p>We're sorry, but there was an error processing your request.</p>\n"
    //                     "</body>\n"
    //                     "</html>";
    //                     respo_body += p;
}

void Response::setSrvDefErrPage()
{
    respo_body = getErrPage(500);
}

int Response::BuildHtmlIndx()
{
    DIR *directory = opendir(_target.c_str());
    if (directory == NULL)
    {
        std::cerr << "opendir failed" << std::endl;
        return (1);
    }

    std::string dirListPage;
    dirListPage.append("<html>\n");
    dirListPage.append("<head>\n");
    dirListPage.append("<title> Index of " + _target + "</title>\n");
    dirListPage.append("</head>\n");
    dirListPage.append("<body>\n");
    dirListPage.append("<h1> Index of " + _target + "</h1>\n");
    dirListPage.append("<table>\n");
    dirListPage.append("<tr><th>File Name</th><th>Last Modification</th><th>File Size</th></tr>\n");

    struct dirent *entityStruct;
    while ((entityStruct = readdir(directory)) != NULL)
    {
        if (strcmp(entityStruct->d_name, ".") == 0)
            continue;

        std::string file_path = _target + entityStruct->d_name;
        struct stat file_stat;
        stat(file_path.c_str(), &file_stat);

        dirListPage.append("<tr>\n");
        dirListPage.append("<td>");
        dirListPage.append("<a href=\"" + std::string(entityStruct->d_name) + "\">" + std::string(entityStruct->d_name) + "</a>");
        if (S_ISDIR(file_stat.st_mode))
            dirListPage.append("/");
        dirListPage.append("</td>\n");
        dirListPage.append("<td>" + std::string(ctime(&file_stat.st_mtime)) + "</td>\n");
        dirListPage.append("<td>");
        if (!S_ISDIR(file_stat.st_mode))
            dirListPage.append(toString(file_stat.st_size));
        dirListPage.append("</td>\n");
        dirListPage.append("</tr>\n");
    }
    dirListPage.append("</table>\n");
    dirListPage.append("</body>\n");
    dirListPage.append("</html>\n");

    body_data.assign(dirListPage.begin(), dirListPage.end());
    body_len = body_data.size();
    return 0;
}

std::string Response::_sCodeStr(short _sCode)
{
    switch (_sCode)
    {
    case 100:
        return "Continue";
    case 200:
        return "OK";
    case 300:
        return "Multiple Choices";
    case 400:
        return "Bad Request";
    case 401:
        return "Unauthorized";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 500:
        return "Internal Server Error";
    default:
        return "Undefined";
    }
}

int Response::readFile()
{
    std::ifstream file(_target.c_str());
    if (file.fail())
    {
        _scode = 404;
        return 1;
    }
    
    std::ostringstream ss;
    ss << file.rdbuf();
    respo_body = ss.str();
    return 0;
}

void Response::BuildErrPage(Request &req_)
{
    ServerConfig::Server &srv_ = cnf->serverConfigs[req_.srv_index];
    ServerConfig::LocationConfig &loc_ = srv_.locations[req_.location_index];

    if (!srv_.errorPage.count(_scode) || srv_.errorPage.at(_scode).empty() ||
        req_.get_met() == DELETE || req_.get_met() == POST)
    {
        setSrvDefErrPage();
    }
    else
    {
        if (_scode >= 400 && _scode < 500)
        {
            location_header = srv_.errorPage[_scode];
            if (location_header[0] != '/')
                location_header.insert(location_header.begin(), '/');
            _scode = 300;
        }
        _target = loc_.getPath() + srv_.errorPage[_scode];
        short _old_scode = _scode;
        if (readFile())
        {
            _scode = _old_scode;
            respo_body = getErrPage(_scode);
        }
    }
}

static std::string comb_Paths(const std::string &basePath, const std::string &middlePath, const std::string &endPath)
{
    std::string result;
    int baseLength = basePath.length();
    int middleLength = middlePath.length();

    // Ensure correct separators and remove duplicate separators
    if (baseLength > 0 && basePath[baseLength - 1] == '/' && !middlePath.empty() && middlePath[0] == '/')
    {
        result = basePath.substr(0, baseLength - 1); // Remove trailing '/'
    }
    else
    {
        result = basePath;
        if (baseLength > 0 && basePath[baseLength - 1] != '/')
        {
            result += '/';
        }
    }

    if (middleLength > 0 && middlePath[0] == '/')
    {
        result += middlePath.substr(1); // Remove leading '/'
    }
    else
    {
        result += middlePath;
        if (middleLength > 0 && middlePath[middleLength - 1] != '/')
        {
            result += '/';
        }
    }

    if (!endPath.empty() && endPath[0] == '/')
    {
        result += endPath.substr(1); // Remove leading '/'
    }
    else
    {
        result += endPath;
    }

    return result;
}

static bool isDir(std::string path)
{
    struct stat file;
    if (stat(path.c_str(), &file) != 0)
        return (false);
    return (S_ISDIR(file.st_mode));
}

static void LocationMatch(std::string pth, std::vector<ServerConfig::LocationConfig> &loc, std::string &key)
{
    size_t match = 0;

    for (std::vector<ServerConfig::LocationConfig>::const_iterator it = loc.begin(); it != loc.end(); ++it)
    {
       
        if (pth.find(it->getPath()) == 0)
        {
            // std::cout <<"it:::" << it->getPath() <<std::endl;
            if (it->getPath() == "/" || pth.length() == it->getPath().length() || pth[it->getPath().length()] == '/')
            {
                if (it->getPath().length() > match)
                {
                    match = it->getPath().length();
                    key = it->getPath();
                }
            }
        }
    }
}

static bool is_Method(std::string method, ServerConfig::LocationConfig &location, short &c)
{
    std::vector<std::string> methods = location.methods;

    if ((method == "GET" && methods[0] == "") ||
        (method == "POST" && methods[1] == "") ||
        (method == "DELETE" && methods[2] == ""))
    {
        c = 405;
        return true;
    }
    return false;
}

static bool fileExists(const std::string &f)
{
    std::ifstream file(f.c_str());
    return (file.good());
}

static bool valid_Ret(const ServerConfig::LocationConfig &loc, short &code, std::string &location)
{
    if (!loc.return_.empty())
    {
        code = 300;
        location = loc.return_;
        if (location[0] != '/')
        {
            location.insert(location.begin(), '/');
        }
        return true;
    }
    return false;
}

int Response::srv_target(Request &req)
{
    std::string location_key;
    ServerConfig::Server &_server = cnf->serverConfigs[0];
    LocationMatch(req.get_loc(), _server.locations, location_key);
    if (location_key.length() > 0)
    {
        ServerConfig::LocationConfig target_location = *_server.getLocationKey(location_key);
        
        if (is_Method(req.get_met(), target_location, _scode))
        {
            std::cout << "METHOD NOT ALLOWED \n";
            return (1);
        }
        if (req.get_body().length() > _server.maxBodySize)
        {
            _scode = 413;
            return (1);
        }
        if (valid_Ret(target_location, _scode, location_header))
            return (1);
        if (target_location.getPath().find("cgi-bin") != std::string::npos)
        {
        //     // Placeholder for CGI handling
        //     // return (handleCgi(location_key));
        }

        if (!target_location.root.empty())
        {
            _target = comb_Paths(target_location.root,req.get_loc(), "");
            // Replace alias logic or any other required processing
        }
        else
        {
            _target = comb_Paths(_server.super_root ,req.get_loc(), "");
        }
        // if (!target_location.cgiPath.empty())
        // {
        //     // Check for CGI extension and handle accordingly
        // }
        if (isDir(_target))
        {
            if (_target.back() != '/')
            {
                _scode = 301;
                location_header = req.get_loc() + "/";
                return (1);
            }

            if (!target_location.loc_index.empty()){
               _target += target_location.getLocIndx();
            }
            else {
                _target += _server.srv_index;
                // std::cout << _server.srv_index  << ":::::::::::::::::::"<< std::endl;
            }
            if (!fileExists(_target))
            {
                if (target_location.autoindex == "on")
                {
                    _target.erase(_target.find_last_of('/') + 1);
                    aut_indx_flag = true;
                    return (0);
                }
                else
                {
                    _scode = 403;
                    return (1);
                }
            }

            if (isDir(_target))
            {
                _scode = 301;
                location_header = comb_Paths(req.get_loc(), target_location.loc_index, "");
                if (location_header.back() != '/')
                    location_header.push_back('/');

                return (1);
            }
        }
    }
    return (0);
}

int Response::BuildBody(Request &req)
{
    ServerConfig::Server &srv = cnf->serverConfigs[req.srv_index];
    // ServerConfig::LocationConfig &loc = srv.locations[req.location_index];

    if (req.get_body().length() > (srv.maxBodySize))
    {
        _scode = 413;
        return (1);
    }
    if (srv_target(req))
        return (1);
    if (cgi_flag || aut_indx_flag)
        return (0);
    if (req.get_met() == GET)
    {
    if (readFile())
        return (1);
    }
    else if (req.get_met() == POST)
    {
        std::cout << "handle post i guess" << std::endl;
    }
    else if (req.get_met() == DELETE)
    {
        if (!fileExists(_target))
        {
            _scode = 404;
            return (1);
        }
        if (remove(_target.c_str()) != 0)
        {
            _scode = 500;
            return (1);
        }
    }
    _scode = 200;
    return (0);
}

void Response::respo_Cut(size_t i)
{
    payload_content = payload_content.substr(i);
}
