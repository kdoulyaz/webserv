#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "header.hpp"
#include "ContentType.hpp"

class Response
{
    private:
        std::string _target;
        std::vector<uint8_t> body_data;
        size_t body_len;
        std::string respo_body;
        std::string location_header;
        short _scode;
        // char *raw_respo;
        std::string statusMsg;
        int cgi_flag;
        int cgi_pipfd[2];
        size_t cgi_respo_len;
        bool aut_indx_flag;

    public:
        Response();
        ~Response();
        ContentType  contentype;
        std::string         payload_content;
        size_t              get_body_len() const; 
        std::string         get_Content();
        int                 get_sCode() const;
        int                 get_Cgi() const;
        bool                get_AutIndx_Flag() const;
        std::string         get_target() const; 
        std::string         getStatusMsg() const;
        std::string         getErrPage(short ) const;
        std::string         get_respo_body() const;
        void                set_sCode(short scode);
        void                set_bdyTorespo();
        void                set_StatusLin();
        void                SrvErrPages(const ServerConfig::Server &, const short &, const char *);
        void                setResponseHeader(const short &,const std::string &);
        void                setSrvDefErrPage();
        void                addFiled_date();
        void                addFiled_location();
        void                addField_Server();
        void                addField_ConnectionFlag(Request &);
        void                addFiled_ContentLen();
        void                addField_ContentType();
        void                addFields(Request &);
        int                 BuildHtmlIndx();
        void                BuildErrPage(Request &);
        void                respo_Cut(size_t i);
        int                 BuildBody(Request &);
        int                 srv_target(Request &);
        std::string         toString(short val) const;
        int                 readFile();
        std::string         _sCodeStr(short _sCode);
        //variables

};
#endif