/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:19:25 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/29 17:53:14 by shmimi           ###   ########.fr       */
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
    this->servers = this->getAllServers();

    setPort();
    // setServerName(0, "");
    // setRoot(0, "");
    // setErrorPage(0, "");
    // setIndex(0, "");
    // setAutoIndex(0, "");
    // setClientMaxBodySize(0, "");
    // setAllowedMethods(0, "");
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

size_t Config::getPortIndex(const std::string &port)
{
    (void)port;
    for (size_t i = 0; i < this->servers.size(); i++)
    {
        for (size_t j = 0; j < this->servers[i].size(); j++)
        {
            if (this->servers[i][j].first == "listen" && this->servers[i][j].second[0] == port)
            {
                return i;
            }
        }
    }
    return 0;
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

void Config::setServerName(int isLocation, const std::string &uri, const std::string& port)
{
    this->serverName.clear();
    
    size_t portIndex = getPortIndex(port);
    if (!isLocation)
    {
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "server_name")
            {
                this->serverName = this->servers[portIndex][j].second[0];
                return;
            }
        }
    }
    else
    {
        std::vector<std::string> splittedUri = split(uri, "/");
        splittedUri[1] = '/' + splittedUri[1] + '/';
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1], 0) != std::string::npos)
            {
                size_t k = j + 1;
                while(k < this->servers[portIndex].size() && this->servers[portIndex][k].first != "server.location")
                {
                    if (k < this->servers[portIndex].size() && this->servers[portIndex][k].first == "server_name")
                    {
                        this->serverName = this->servers[portIndex][k].second[0];
                        return;
                    }
                    k++;
                }
            }
        } 
        if (this->serverName.size() == 0)
        {
            setClientMaxBodySize(0, "", port);
        }
    }
}

size_t Config::getConfigLocationIndex(const std::string &clientUri, const std::string& port)
{
    size_t i = getPortIndex(port);
    std::vector<std::string> splittedClientUri = split(clientUri, "/");
    splittedClientUri[1] = '/' + splittedClientUri[1] + '/';
    for (; i < this->locations.size(); i++)
    {
        std::vector<std::string> splitteduriConfig = split(this->locations[i].first, "/");
        splitteduriConfig[1] = '/' + splitteduriConfig[1] + '/';
            if (splitteduriConfig[1] == splittedClientUri[1] && clientUri.find(splitteduriConfig[1], 0) != std::string::npos)
            {
                return i;
            }
    }
    return i;
}

void Config::setRoot(int isLocation, const std::string &uri, const std::string& port)
{
    this->root.clear();
    
    size_t portIndex = getPortIndex(port);
    if (!isLocation)
    {
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "root")
            {
                this->root = this->servers[portIndex][j].second[0];
                std::cout << "this->root " << this->root << std::endl; 
                return;
            }
        }
    }
    else
    {
        std::vector<std::string> splittedUri = split(uri, "/");
        splittedUri[1] = '/' + splittedUri[1] + '/';
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1], 0) != std::string::npos)
            {
                size_t k = j + 1;
                while(k < this->servers[portIndex].size() && this->servers[portIndex][k].first != "server.location")
                {
                    if (k < this->servers[portIndex].size() && this->servers[portIndex][k].first == "root")
                    {
                        this->root = this->servers[portIndex][k].second[0];
                        std::cout << "this->root " << this->root << std::endl; 
                        return;
                    }
                    k++;
                }
            }
        } 
        if (this->root.size() == 0)
        {
            setRoot(0, "", port);
        }
    }
    std::cout << "this->root " << this->root << std::endl; 
}

void Config::setIndex(int isLocation, const std::string &uri, const std::string& port)
{
    this->index.clear();
    
    size_t portIndex = getPortIndex(port);
    if (!isLocation)
    {
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "index")
            {
                this->index = this->servers[portIndex][j].second[0];
                return;
            }
        }
    }
    else
    {
        std::vector<std::string> splittedUri = split(uri, "/");
        splittedUri[1] = '/' + splittedUri[1] + '/';
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1], 0) != std::string::npos)
            {
                size_t k = j + 1;
                while(k < this->servers[portIndex].size() && this->servers[portIndex][k].first != "server.location")
                {
                    if (k < this->servers[portIndex].size() && this->servers[portIndex][k].first == "index")
                    {
                        this->index = this->servers[portIndex][k].second[0];
                        return;
                    }
                    k++;
                }
            }
        } 
        if (this->index.size() == 0)
        {
            setIndex(0, "", port);
        }
    }
}

void Config::setErrorPage(int isLocation, const std::string &uri, const std::string& port)
{
    this->errorPage.clear();
    
    size_t portIndex = getPortIndex(port);
    if (!isLocation)
    {
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "error_page")
            {
                this->errorPage = this->servers[portIndex][j].second;
                return;
            }
        }
    }
    else
    {
        std::vector<std::string> splittedUri = split(uri, "/");
        splittedUri[1] = '/' + splittedUri[1] + '/';
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1], 0) != std::string::npos)
            {
                size_t k = j + 1;
                while(k < this->servers[portIndex].size() && this->servers[portIndex][k].first != "server.location")
                {
                    if (k < this->servers[portIndex].size() && this->servers[portIndex][k].first == "error_page")
                    {
                        this->errorPage = this->servers[portIndex][k].second;
                        return;
                    }
                    k++;
                }
            }
        } 
        if (this->errorPage.size() == 0)
        {
            setErrorPage(0, "", port);
        }
    }
}

void Config::setAutoIndex(int isLocation, const std::string &uri, const std::string& port)
{
    this->autoIndex.clear();
    
    size_t portIndex = getPortIndex(port);
    if (!isLocation)
    {
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "autoindex")
            {
                this->autoIndex = this->servers[portIndex][j].second[0];
                return;
            }
        }
    }
    else
    {
        std::vector<std::string> splittedUri = split(uri, "/");
        splittedUri[1] = '/' + splittedUri[1] + '/';
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1], 0) != std::string::npos)
            {
                size_t k = j + 1;
                while(k < this->servers[portIndex].size() && this->servers[portIndex][k].first != "server.location")
                {
                    if (k < this->servers[portIndex].size() && this->servers[portIndex][k].first == "autoindex")
                    {
                        this->autoIndex = this->servers[portIndex][k].second[0];
                        return;
                    }
                    k++;
                }
            }
        } 
        if (this->autoIndex.size() == 0)
        {
            setAutoIndex(0, "", port);
        }
    }
}

void Config::setClientMaxBodySize(int isLocation, const std::string &uri, const std::string& port)
{
    this->clientMaxBodySize.clear();
    
    size_t portIndex = getPortIndex(port);
    if (!isLocation)
    {
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "client_max_body_size")
            {
                this->clientMaxBodySize = this->servers[portIndex][j].second[0];
                return;
            }
        }
    }
    else
    {
        std::vector<std::string> splittedUri = split(uri, "/");
        splittedUri[1] = '/' + splittedUri[1] + '/';
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1], 0) != std::string::npos)
            {
                size_t k = j + 1;
                while(k < this->servers[portIndex].size() && this->servers[portIndex][k].first != "server.location")
                {
                    if (k < this->servers[portIndex].size() && this->servers[portIndex][k].first == "client_max_body_size")
                    {
                        this->clientMaxBodySize = this->servers[portIndex][k].second[0];
                        return;
                    }
                    k++;
                }
            }
        } 
        if (this->clientMaxBodySize.size() == 0)
        {
            setClientMaxBodySize(0, "", port);
        }
    }
}

void Config::setUploadDir(int isLocation, const std::string &uri, const std::string& port)
{
    this->uploadDir.clear();
    
    size_t portIndex = getPortIndex(port);
    if (!isLocation)
    {
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "upload_dir")
            {
                this->uploadDir = this->servers[portIndex][j].second[0];
                return;
            }
        }
    }
    else
    {
        std::vector<std::string> splittedUri = split(uri, "/");
        splittedUri[1] = '/' + splittedUri[1] + '/';
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1], 0) != std::string::npos)
            {
                size_t k = j + 1;
                while(k < this->servers[portIndex].size() && this->servers[portIndex][k].first != "server.location")
                {
                    if (k < this->servers[portIndex].size() && this->servers[portIndex][k].first == "upload_dir")
                    {
                        this->uploadDir = this->servers[portIndex][k].second[0];
                        return;
                    }
                    k++;
                }
            }
        } 
        if (this->uploadDir.size() == 0)
        {
            setUploadDir(0, "", port);
        }
    }
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

void Config::setAllowedMethods(int isLocation, const std::string &uri, const std::string& port)
{
    std::map<std::string, int> allowedMethodsGlobal = this->allowedMethods;
    this->allowedMethods.clear();
    if (!isLocation)
    {
        for (size_t i = 0; i < this->globalDirectives.size(); i++)
        {
            if (this->globalDirectives[i].first == "allowed_methods")
            {
                for (size_t j = 0; j < this->globalDirectives[i].second.size(); j++)
                {
                    this->allowedMethods[this->globalDirectives[i].second[j]] = 1;
                }
            }
        }
    }
    else
    {
        std::vector<std::string> splittedUri = split(uri, "/");
        splittedUri[1] = '/' + splittedUri[1] + '/';
        for (size_t i = 0; i < locations.size(); i++)
        {
            if (uri.find(splittedUri[1], 0) != std::string::npos && i >= getConfigLocationIndex(uri, port))
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
        }
    }
}

int Config::isLocation(const std::string &uri, const std::string& port)
{
    size_t index = getPortIndex(port);
    std::vector<std::string> splittedUri = split(uri, "/");
    splittedUri[1] = '/' + splittedUri[1] + '/';
    for (size_t j = 0; j < this->servers[index].size(); j++)
    {
        if (this->servers[index][j].first == "server.location" && this->servers[index][j].second[0].find(splittedUri[1], 0) != std::string::npos)
        {
            std::cout << "uri " << splittedUri[1] << "    " << this->servers[index][j].second[0] << std::endl;
            return 1;
        }
    }
    return 0;
}
