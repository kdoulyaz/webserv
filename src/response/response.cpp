#include "../../include/response.hpp"

Response::Response()
{
}
Response::~Response()
{
}
std::string Response::getContentType(std::string &filename)
{
    if (filename.empty())
        return WALO;
    std::map<std::string, std::string> contentTypeMap;
    contentTypeMap[".html"] = "text/html";
    contentTypeMap[".png"] = "image/png";
    contentTypeMap[".mp4"] = "video/mp4";
    contentTypeMap[".mp3"] = "audio/mpeg";
    contentTypeMap[".css"] = "text/css";
    contentTypeMap[".js"] = "text/javascript";
    contentTypeMap[".json"] = "application/json";
    contentTypeMap[".xml"] = "application/xml";
    contentTypeMap[".pdf"] = "application/pdf";
    contentTypeMap[".zip"] = "application/zip";
    contentTypeMap[".txt"] = "text/plain";
    contentTypeMap[".gif"] = "image/gif";
    contentTypeMap[".jpg"] = "image/jpeg";
    contentTypeMap[".svg"] = "image/svg+xml";
    contentTypeMap[".wav"] = "audio/wav";
    contentTypeMap[".mpg"] = "video/mpeg";
    contentTypeMap[".mov"] = "video/quicktime";
    contentTypeMap[".avi"] = "video/x-msvideo";
    contentTypeMap[".php"] = "php";
    contentTypeMap[".py"] = "python";

    std::string contentType = "application/octet-stream";                // default content type is binary
    std::string extension = filename.substr(filename.find_last_of(NO9TA));
    if (contentTypeMap.count(extension) > 0)
    {
        contentType = contentTypeMap[extension];
        return contentType;
    }
    return contentType;
}

void Response::send_res(Network *net, std::string key)
{
    std::ostringstream contentLengthHeader;
    std::string response;

    if (!net->header_sent)
    {
        if (key == "404:")
            net->file.open("root/error/404.html");
        else
            net->file.open(net->_file_name.c_str());
        if (!net->file.is_open() && !net->_file_name.empty())
        {
            std::cout << "Error opening file" << net->_file_name << std::endl;
            net->is_done = true;
            return;
        }

        // Get file size
        net->file.seekg(0, std::ios::end);
        net->file_size = net->file.tellg();
        net->file.seekg(0, std::ios::beg);

        // Create response header
        if (key == "404:"){
            response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n";
            contentLengthHeader << "Content-Length: " << net->file_size << "\r\n\r\n";
            response += contentLengthHeader.str();
        }
        else{
            response = "HTTP/1.1 200 OK\r\nContent-Type: " + getContentType(net->_file_name) + "\r\n";
            contentLengthHeader << "Content-Length: " << net->file_size << "\r\n\r\n";
            response += contentLengthHeader.str();
        }

        if (send(net->get_socket_fd(), response.c_str(), response.size(), 0) < 1)
        {
            std::cout << "Error sending response header" << std::endl;
            net->is_done = true;
            net->file.close();
            return;
        }

        net->header_sent = true;
    }
    else
    {
        const int buff_size = 1024;
        char buffer[buff_size];

        net->file.read(buffer, buff_size);
        int bytes_read = net->file.gcount();

        if (bytes_read == -1)
        {
            std::cout << "Error reading file" << std::endl;
            net->is_done = true;
            net->file.close();
            return;
        }

        if (bytes_read < buff_size)
        {
            if (send(net->get_socket_fd(), buffer, bytes_read, 0) == -1)
                std::cout << "error sending response2" << std::endl;
            net->is_done = true;
            net->file.close();
            return;
        }

        if (send(net->get_socket_fd(), buffer, bytes_read, 0) < 1)
        {
            std::cout << "Error sending response" << std::endl;
            net->file.close();
            return;
        }

        net->file_size -= bytes_read;
    }
}