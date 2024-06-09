#include "Default.hpp"

Default::Default() {}

Default::~Default() {}

const std::string getErrorMessage(const std::string &errorCode)
{
    int error = std::stoi(errorCode);
    switch (error)
    {
    case 400:
        return "Bad Request";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 413:
        return "Payload Too Large";
    case 500:
        return "Internal Server Error";
    case 502:
        return "Bad Gateway";
    case 504:
        return "Gateway Timeout";
    case 505:
        return "HTTP Version Not Supported";
    default:
        return "Forbidden";
    }
}

const std::string Default::generateIndexPage()
{
    std::string indexPage = "\
    <html>\
        <head>\
            <title>Welcome to Nkhinx!</title>\
            <style>\
            html { color-scheme: light dark; }\
            body { width: 35em; margin: 0 auto;\
            font-family: Tahoma, Verdana, Arial, sans-serif; }\
            </style>\
            </head>\
            <body>\
            <h1>Welcome to Nkhinx!</h1>\
            <p>If you see this page, the Nkhinx web server is successfully installed and\
            working. Further configuration is required.</p>\
            <p><em>Thank you for using Nkhinx.</em></p>\
        </body>\
    </html>";
    return indexPage;
}

const std::string Default::generateErrorPage(const std::string &errorCode)
{
    std::string errorMsg = getErrorMessage(errorCode);
    
        std::string errorPage = "\
    <html>\
        <head><title>" + errorCode + " " + errorMsg + "</title></head>\
        <body>\
        <center><h1>" + errorCode + " " + errorMsg + "</h1></center>\
        <hr><center>nkhinx/1.0.0</center>\
        </body>\
    </html>\
    ";
    return errorPage;
}
