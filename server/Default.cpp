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
