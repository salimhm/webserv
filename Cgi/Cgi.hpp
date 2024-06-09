#pragma once

#include "../client/Client.hpp"
#include "../parsing/Config.hpp"
#include "../client/clientHandler.hpp"

class Cgi
{
    private:
    std::string request;

    public:

    Cgi(std::string& request);
    const std::string postCgi(Client& client, Config &config);
    void getCgi(Client& client);
};
class Client;