/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:19:25 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/26 17:47:05 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <sstream>
#include "../client/Client.hpp"

std::string Config::nullFile;
Config::Config() : filePath(nullFile) {}

Config::Config(const std::string &filePath) : Parser(filePath), filePath(filePath)
{
    // std::ifstream file(filePath);
    // if (!file.is_open()) {
    //     throw std::runtime_error("Failed to open config file");
    // }
    this->parse();
    this->globalDirectives = this->getGlobalDirectives();
    this->locations = this->getLocations();

    setPort();
    setServerName(0, "");
    setRoot(0, "");
    setErrorPage(0, "");
    setIndex(0, "");
    setAutoIndex(0, "");
    setClientMaxBodySize(0, "");
    // getPort();
}

Config::Config(const Config &cpy) : filePath(cpy.filePath)
{
    *this = cpy;
}
Config &Config::operator=(const Config &cpy)
{
    if (this == &cpy)
        return *this;
    return *this;
}

void Config::setPort()
{
    std::vector<int> ports;
    // std::vector<std::pair<std::string, std::vector<std::string> > > globalDirectives = this->getGlobalDirectives();
    for (size_t i = 0; i < globalDirectives.size(); i++)
    {
        if (globalDirectives[i].first == "listen")
        {
            int port;
            std::istringstream(globalDirectives[i].second[0]) >> port;
            ports.push_back(port);
        }
    }
    this->port = ports;
    // return ports;
}

void Config::setServerName(int isLocation, const std::string &uri)
{
    std::string serverName;
    if (!isLocation)
    {
        for (size_t i = 0; i < globalDirectives.size(); i++)
        {
            if (globalDirectives[i].first == "server_name")
            {
                serverName = globalDirectives[i].second[0];
            }
        }
    }
    else
    {
        for (size_t i = 0; i < locations.size(); i++)
        {
            if (this->locations[i].first == uri)
            {
                // std::cout << locations[i].second.first << std::endl;
                if (this->locations[i].second.first == "server_name")
                {
                    for (size_t j = 0; j < this->locations[i].second.second.size(); j++)
                    {
                        // std::cout << this->locations[i].second.first << " ==> " << this->locations[i].second.second[j] << j << std::endl;
                        this->serverName = this->locations[i].second.second[0];
                        return;
                    }
                }
            }
        }
    }
    this->serverName = serverName;
}

void Config::setRoot(int isLocation, const std::string &uri)
{
    std::string root;
    if (!isLocation)
    {
        for (size_t i = 0; i < this->globalDirectives.size(); i++)
        {
            if (this->globalDirectives[i].first == "root")
            {
                root = this->globalDirectives[i].second[0];
            }
        }
    }
    else
    {
        for (size_t i = 0; i < locations.size(); i++)
        {
            if (this->locations[i].first == uri)
            {
                // std::cout << locations[i].second.first << std::endl;
                if (this->locations[i].second.first == "root")
                {
                    for (size_t j = 0; j < this->locations[i].second.second.size(); j++)
                    {
                        // std::cout << this->locations[i].second.first << " ==> " << this->locations[i].second.second[j] << j << std::endl;
                        this->root = this->locations[i].second.second[0];
                        return;
                    }
                }
            }
        }
    }
    this->root = root;
}

void Config::setIndex(int isLocation, const std::string &uri)
{
    std::string index;
    if (!isLocation)
    {
        for (size_t i = 0; i < this->globalDirectives.size(); i++)
        {
            if (this->globalDirectives[i].first == "index")
            {
                index = this->globalDirectives[i].second[0];
            }
        }
    }
    else
    {
        for (size_t i = 0; i < locations.size(); i++)
        {
            if (this->locations[i].first == uri)
            {
                // std::cout << locations[i].second.first << std::endl;
                if (this->locations[i].second.first == "index")
                {
                    for (size_t j = 0; j < this->locations[i].second.second.size(); j++)
                    {
                        // std::cout << this->locations[i].second.first << " ==> " << this->locations[i].second.second[j] << j << std::endl;
                        this->index = this->locations[i].second.second[0];
                        return;
                    }
                }
            }
        }
    }
    this->index = index;
}

void Config::setErrorPage(int isLocation, const std::string &uri)
{
    std::vector<std::string> errorPage;
    if (!isLocation)
    {
        for (size_t i = 0; i < this->globalDirectives.size(); i++)
        {
            if (this->globalDirectives[i].first == "errorPage")
            {
                errorPage = this->globalDirectives[i].second;
            }
        }
    }
    else
    {
        for (size_t i = 0; i < locations.size(); i++)
        {
            if (this->locations[i].first == uri)
            {
                // std::cout << locations[i].second.first << std::endl;
                if (this->locations[i].second.first == "errorPage")
                {
                    for (size_t j = 0; j < this->locations[i].second.second.size(); j++)
                    {
                        // std::cout << this->locations[i].second.first << " ==> " << this->locations[i].second.second[j] << j << std::endl;
                        this->errorPage = this->locations[i].second.second;
                        return;
                    }
                }
            }
        }
    }
    this->errorPage = errorPage;
}

void Config::setAutoIndex(int isLocation, const std::string &uri)
{
    std::string autoIndex;
    if (!isLocation)
    {
        for (size_t i = 0; i < this->globalDirectives.size(); i++)
        {
            if (this->globalDirectives[i].first == "autoindex")
            {
                autoIndex = this->globalDirectives[i].second[0];
            }
        }
    }
    else
    {
        for (size_t i = 0; i < locations.size(); i++)
        {
            if (this->locations[i].first == uri)
            {
                // std::cout << locations[i].second.first << std::endl;
                if (this->locations[i].second.first == "autoindex")
                {
                    for (size_t j = 0; j < this->locations[i].second.second.size(); j++)
                    {
                        // std::cout << this->locations[i].second.first << " ==> " << this->locations[i].second.second[j] << j << std::endl;
                        this->autoIndex = this->locations[i].second.second[0];
                        return;
                    }
                }
            }
        }
    }
    this->autoIndex = autoIndex;
}

void Config::setClientMaxBodySize(int isLocation, const std::string &uri)
{
    std::string clientMaxBodySize;
    if (!isLocation)
    {
        for (size_t i = 0; i < this->globalDirectives.size(); i++)
        {
            if (this->globalDirectives[i].first == "client_max_body_size")
            {
                autoIndex = this->globalDirectives[i].second[0];
            }
        }
    }
    else
    {
        for (size_t i = 0; i < locations.size(); i++)
        {
            if (this->locations[i].first == uri)
            {
                // std::cout << locations[i].second.first << std::endl;
                if (this->locations[i].second.first == "client_max_body_size")
                {
                    for (size_t j = 0; j < this->locations[i].second.second.size(); j++)
                    {
                        // std::cout << this->locations[i].second.first << " ==> " << this->locations[i].second.second[j] << j << std::endl;
                        this->autoIndex = this->locations[i].second.second[0];
                        return;
                    }
                }
            }
        }
    }
    this->clientMaxBodySize = clientMaxBodySize;
}

const std::vector<int> Config::getPort()
{
    return this->port;
}
const std::string Config::getServerName()
{
    return this->serverName;
}
const std::string Config::getRoot()
{
    return this->root;
}
const std::vector<std::string> Config::getErrorPage()
{
    return this->errorPage;
}
const std::string Config::getIndex()
{
    return this->index;
}
const std::string Config::getAutoIndex()
{
    return this->autoIndex;
}
const std::string Config::getClientMaxBodySize()
{
    return this->clientMaxBodySize;
}

const std::map<std::string, int> Config::getAllowedMethods()
{
    return this->allowedMethods;
}

std::string Config::getErrorCode()
{
    for (size_t i = 0; i < this->globalDirectives.size(); i++)
    {
        if (this->globalDirectives[i].first == "error_page")
        {
            return this->globalDirectives[i].second[0];
        }
    }
    return "";
}

std::string Config::getErrorPage(const std::string &errorCode, const std::string &uri, int isLocation)
{
    // int flag = 1;
    if (isLocation)
    {
        for (size_t i = 0; i < locations.size(); i++)
        {
            if (this->locations[i].first == uri)
            {
                std::cout << locations[i].second.first << std::endl;
                if (this->locations[i].second.first == "error_page")
                {
                    for (size_t j = 0; j < this->locations[i].second.second.size(); j++)
                    {
                        // std::cout << this->locations[i].second.first << " ==> " << this->locations[i].second.second[j] << std::endl;
                        return this->locations[i].second.second[j + 1];
                    }
                }
            }
        }
    }
    else
    {
        for (size_t i = 0; i < this->globalDirectives.size(); i++)
        {
            if (this->globalDirectives[i].first == "error_page")
            {
                for (size_t j = 0; j < this->globalDirectives[i].second.size(); j++)
                {
                    if (this->globalDirectives[i].second[j] == errorCode)
                        return this->globalDirectives[i].second[j + 1];
                }
            }
        }
    }
    return "";
}

void Config::setAllowedMethods(int isLocation, const std::string &uri)
{
    this->allowedMethods.clear();
    if (!isLocation)
    {
        for (size_t i = 0; i < this->globalDirectives.size(); i++)
        {
            if (this->globalDirectives[i].first == "allowed_methods")
            {
                // std::cout << "Global " << this->globalDirectives[i].second.size() << std::endl;
                for (size_t j = 0; j < this->globalDirectives[i].second.size(); j++)
                {
                    this->allowedMethods[this->globalDirectives[i].second[j]] = 1;
                }
            }
        }
        return;
        for (std::map<std::string, int>::iterator it = this->allowedMethods.begin(); it != this->allowedMethods.end(); it++)
        {
            std::cout << it->first << "==> " << it->second << std::endl;
        }
    }
    else
    {
        for (size_t i = 0; i < locations.size(); i++)
        {
            std::cout << locations[i].second.first << std::endl;
            if (this->locations[i].first == uri)
            {
                if (this->locations[i].second.first == "allowed_methods")
                {
                    // std::cout << "Local " << this->locations[i].second.second.size() << std::endl;
                    for (size_t j = 0; j < this->locations[i].second.second.size(); j++)
                    {
                        // std::cout << this->locations[i].second.first << " ==> " << this->locations[i].second.second[j] << std::endl;
                        this->allowedMethods[this->locations[i].second.second[j]] = 1;
                    }
                }
            }
        }
        return;
        for (std::map<std::string, int>::iterator it = this->allowedMethods.begin(); it != this->allowedMethods.end(); it++)
        {
            // std::cout << "Local\n";
            std::cout << it->first << "==> " << it->second << std::endl;
        }
    }
}

int Config::isLocation(const std::string &uri)
{
    for (size_t i = 0; i < this->locations.size(); i++)
    {
        if (this->locations[i].first == uri)
            return 1;
    }
    return 0;
}

// void Config::overrideConfig(std::string& uri, Client client)
// {
//     for (size_t i = 0; i < this->locations.size(); i++)
//     {
//         if (this->locations[i].first == uri)
//         {
//             for (size_t j = 0; j < this->locations[i].second.second.size(); j++)
//             {
//                 if (this->locations[i].second.first == )
//             }
//         }
//     }
// }