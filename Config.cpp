/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:19:25 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/05 17:32:09 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <sstream>

Config::Config() {}
Config::Config(const Config &cpy)
{
    *this = cpy;
}
Config &Config::operator=(const Config &cpy)
{
    if (this == &cpy)
        return *this;
    return *this;
}

const std::vector<int> Config::getPort() const
{
    int port = 0;
    std::ifstream config("webserv.conf");
    std::string line;
    std::vector<int> ports;
    while (getline(config, line))
    {
        if (line.find("listen") != std::string::npos)
        {
            line = line.substr(line.find(" ") + 1);
            std::istringstream(line) >> port;
            ports.push_back(port);
        }
    }
    return ports;
}

const std::string Config::getServerName() const
{
    std::ifstream config("webserv.conf");
    std::string line;
    std::string serverName;
    while (getline(config, line))
    {
        if (line.find("server_name") != std::string::npos)
        {
            int pos = line.find(";");
            serverName = line.substr(line.find(" ") + 1, pos - line.find(" ") - 1);
            break;
        }
    }
    return serverName;
}

const std::string Config::getRoot() const
{
    std::ifstream config("webserv.conf");
    std::string line;
    std::string root;
    while (getline(config, line))
    {
        if (line.find("root") != std::string::npos)
        {
            int pos = line.find(";");
            root = line.substr(line.find(" ") + 1, pos - line.find(" ") - 1);
            break;
        }
    }
    return root;
}

const std::vector<std::string> Config::getIndex() const
{
    std::ifstream config("webserv.conf");
    std::vector<std::string> index;
    std::string line;
    while (getline(config, line))
    {
        if (line.find("index") != std::string::npos)
        {
            std::vector<std::string> splitted = split(line, " ");
            for (size_t i = 1; i < splitted.size(); i++)
            {
                if (i == splitted.size() - 1)
                {
                    int pos = splitted[i].find(";");
                    splitted[i] = splitted[i].substr(0, pos);
                }
                index.push_back(splitted[i]);
                std::cout << splitted[i] << std::endl;
            }
            break;
        }
    }
    return index;
}

const std::map<int, std::string> Config::getErrorPage() const
{
    std::ifstream config("webserv.conf");
    std::string line;
    std::map<int, std::string> errorPage;
    while (getline(config, line))
    {
        if (line.find("error_page") != std::string::npos)
        {
            int error = 0;
            std::vector<std::string> splitted = split(line, " ");
            std::istringstream(splitted[1]) >> error;

            for (size_t i = 1; i < splitted.size(); i++)
            {
                if (i == splitted.size() - 1)
                {
                    int pos = splitted[i].find(";");
                    splitted[i] = splitted[i].substr(0, pos);
                }
                errorPage[error] = splitted[2];
            }
        }
    }
    std::map<int, std::string>::const_iterator it = errorPage.begin();
    while (it != errorPage.end())
    {
        std::cout << it->first << " => " << it->second << std::endl;
        it++;
    }
    return errorPage;
}

const std::string Config::getAutoIndex() const
{
    std::ifstream config("webserv.conf");
    std::string line;
    std::string autoIndex;
    while (getline(config, line))
    {
        if (line.find("autoindex") != std::string::npos)
        {
            int pos = line.find(";");
            autoIndex = line.substr(line.find(" ") + 1, pos - line.find(" ") - 1);
            break;
        }
    }
    return autoIndex;
}

int Config::getClientMaxBodySize() const
{
    std::ifstream config("webserv.conf");
    std::string line;
    int clientMaxBodySize = 0;
    while (getline(config, line))
    {
        if (line.find("client_max_body_size") != std::string::npos)
        {
            // int pos = line.find(";");
            std::vector<std::string> splitted = split(line, " ");
            std::istringstream(splitted[1]) >> clientMaxBodySize;
            std::cout << clientMaxBodySize << std::endl;
            break;
        }
    }
    return clientMaxBodySize;
}