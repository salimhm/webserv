#pragma once

#include "../client/Client.hpp"
#include "../parsing/Config.hpp"
#include "../client/clientHandler.hpp"

class Cgi
{
    private:
    std::string request;
    int filefd;

    public:

    Cgi(int filefd, std::string& request);
    void postCgi(Client& client, Config &config);
    void getCgi(Client& client);
};
class Client;