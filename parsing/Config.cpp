/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:19:25 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/27 12:32:54 by shmimi           ###   ########.fr       */
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
    setAllowedMethods(0, "");
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
}

void Config::setServerName(int isLocation, const std::string &uri)
{
    std::string serverNameGlobal = this->serverName;
    this->serverName.clear();
    if (!isLocation)
    {
        for (size_t i = 0; i < globalDirectives.size(); i++)
        {
            if (globalDirectives[i].first == "server_name")
            {
                this->serverName = globalDirectives[i].second[0];
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
        if (this->serverName.size() == 0)
        {
            this->serverName = serverNameGlobal;
        }
    }
}

void Config::setRoot(int isLocation, const std::string &uri)
{
    std::string rootGlobal = this->root;
    this->root.clear();
    if (!isLocation)
    {
        for (size_t i = 0; i < this->globalDirectives.size(); i++)
        {
            if (this->globalDirectives[i].first == "root")
            {
                this->root = this->globalDirectives[i].second[0];
            }
        }
    }
    else
    {
        for (size_t i = 0; i < locations.size(); i++)
        {
            if (this->locations[i].first == uri)
            {
                if (this->locations[i].second.first == "root")
                {
                    for (size_t j = 0; j < this->locations[i].second.second.size(); j++)
                    {
                        this->root = this->locations[i].second.second[0];
                        return;
                    }
                }
            }
        }
        if (this->root.size() == 0)
        {
            this->root = rootGlobal;
            return;
        }
    }
}

void Config::setIndex(int isLocation, const std::string &uri)
{
    std::string indexGlobal = this->index;
    this->index.clear();
    if (!isLocation)
    {
        for (size_t i = 0; i < this->globalDirectives.size(); i++)
        {
            if (this->globalDirectives[i].first == "index")
            {
                this->index = this->globalDirectives[i].second[0];
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
        if (this->index.size() == 0)
        {
            this->index = indexGlobal;
        }
    }
}

void Config::setErrorPage(int isLocation, const std::string &uri)
{
    std::vector<std::string> errorPageGlobal = this->errorPage;
    this->errorPage.clear();
    if (!isLocation)
    {
        for (size_t i = 0; i < this->globalDirectives.size(); i++)
        {
            if (this->globalDirectives[i].first == "error_page")
            {
                this->errorPage = this->globalDirectives[i].second;
            }
        }
    }
    else
    {
        std::vector<std::string> splittedUri = split(uri, "/");
        splittedUri[1] = '/' + splittedUri[1] + '/';
        std::cout << "splitted   " << splittedUri[1] << "      " << uri << std::endl;
        for (size_t i = 0; i < locations.size(); i++)
        {
            if (this->locations[i].first == uri || uri.find(splittedUri[1], 0) != std::string::npos)
            {
                if (this->locations[i].second.first == "error_page")
                {
                    for (size_t j = 0; j < this->locations[i].second.second.size(); j++)
                    {
                        this->errorPage = this->locations[i].second.second;
                        return;
                    }
                }
            }
        }
        if (this->errorPage.size() == 0)
        {
            this->errorPage = errorPageGlobal;
        }
    }
    // std::cout << this->errorPage.size() << std::endl;
}

void Config::setAutoIndex(int isLocation, const std::string &uri)
{
    std::string autoIndexGlobal = this->autoIndex;
    this->autoIndex.clear();
    if (!isLocation)
    {
        for (size_t i = 0; i < this->globalDirectives.size(); i++)
        {
            if (this->globalDirectives[i].first == "autoindex")
            {
                this->autoIndex = this->globalDirectives[i].second[0];
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
        if (this->autoIndex.size() == 0)
            this->autoIndex = autoIndexGlobal;
    }
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

void Config::setAllowedMethods(int isLocation, const std::string &uri)
{
    std::map<std::string, int> allowedMethodsGlobal = this->allowedMethods;
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
        // for (std::map<std::string, int>::iterator it = this->allowedMethods.begin(); it != this->allowedMethods.end(); it++)
        // {
        //     std::cout << it->first << "==> " << it->second << std::endl;
        // }
    }
    else
    {
        for (size_t i = 0; i < locations.size(); i++)
        {
            // std::cout << locations[i].second.first << std::endl;
            if (this->locations[i].first == uri)
            {
                if (this->locations[i].second.first == "allowed_methods")
                {
                    for (size_t j = 0; j < this->locations[i].second.second.size(); j++)
                    {
                        this->allowedMethods[this->locations[i].second.second[j]] = 1;
                    }
                }
            }
        }
        if (this->allowedMethods.size() == 0)
        {
            this->allowedMethods = allowedMethodsGlobal;
            return;
        }
        // for (std::map<std::string, int>::iterator it = this->allowedMethods.begin(); it != this->allowedMethods.end(); it++)
        // {
        //     // std::cout << "Local\n";
        //     std::cout << it->first << "==> " << it->second << std::endl;
        // }
    }
}

int Config::isLocation(const std::string &uri)
{
    std::string uriCheck;
    // std::vector<std::string> splittedUri = split(uri, "/");
    // std::cout << "splitted   " << splittedUri[1] << "   " << uri << "   " << this->locations[0].first << std::endl;
    
    // for (size_t i = 0; i < this->locations.size(); i++)
    // {
        
    // }
    for (size_t i = 0; i < this->locations.size(); i++)
    {
        uriCheck += this->locations[i].first;
        if (this->locations[i].first == uri || this->locations[i].first == uriCheck)
        {
            std::cout  << "uriCheck   " << uriCheck << "  " << this->locations[i].first << std::endl;
            return 1;
        }
        uriCheck.clear();
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