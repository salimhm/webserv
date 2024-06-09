/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:19:25 by shmimi            #+#    #+#             */
/*   Updated: 2024/06/09 02:07:07 by shmimi           ###   ########.fr       */
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
    setHost();
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
    for (size_t i = 0; i < this->servers.size(); i++)
    {
        for (size_t j = 0; j < this->servers[i].size(); j++)
        {
            for (size_t p = 0; p < this->servers[i][j].second.size(); p++)
            {
                if (this->servers[i][j].second[p] == port)
                {
                // std::cout << "this->servers[i][j].second[p] " << this->servers[i][j].second[p] << "   " << port << std::endl;
                    return i;
                }
            }
        }
    }
    // std::cout << "?????????????????? " << port << std::endl;
    return 0;
}

void Config::setRedirect(int isLocation, const std::string &uri, const std::string& port)
{
    this->redirect.clear();
    
    size_t portIndex = getPortIndex(port);
    std::cout << "port>>" << port << std::endl;
    if (!isLocation)
    {
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "redirect")
            {                
                this->redirect = this->servers[portIndex][j].second;
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
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1].c_str(), 0, splittedUri[1].length()) != std::string::npos)
            {
                size_t k = j + 1;
                while(k < this->servers[portIndex].size() && this->servers[portIndex][k].first != "server.location")
                {
                    if (k < this->servers[portIndex].size() && this->servers[portIndex][k].first == "redirect")
                    {
                        this->redirect = this->servers[portIndex][k].second;
                        return;
                    }
                    k++;
                }
            }
        }
        for (size_t i = 0; i < this->servers[portIndex].size(); i++)
        {
            if (this->servers[portIndex][i].second[0] == "/")
            {
                size_t j = i + 1;
                while(j < this->servers[portIndex].size() && this->servers[portIndex][j].first != "server.location")
                {
                    if (j < this->servers[portIndex].size() && this->servers[portIndex][j].first == "redirect")
                    {
                        this->redirect = this->servers[portIndex][j].second;
                        return;
                    }
                    j++;
                }
            }
        }
        if (this->redirect.size() == 0)
        {
            setServerName(0, "", port);
        }
    }
}

void Config::setPort()
{
    std::vector<int> ports;
    for (size_t i = 0; i < this->servers.size(); i++)
    {
        for (size_t j = 0; j < this->servers[i].size(); j++)
        {
            if (this->servers[i][j].first == "listen")
            {
                for (size_t p = 0; p < this->servers[i][j].second.size(); p++)
                {
                    int port;
                    std::istringstream(this->servers[i][j].second[p]) >> port;
                    ports.push_back(port);
                }
            }
        }
    }
    this->port = ports;
}

void Config::setHost()
{
    std::vector<std::string> host;
    for (size_t i = 0; i < this->servers.size(); i++)
    {
        for (size_t j = 0; j < this->servers[i].size(); j++)
        {
            if (this->servers[i][j].first == "host")
            {
                // std::cout << "this->servers[i][j].second.size() " << this->servers[i][j].second[0] << std::endl;
                this->host.push_back(this->servers[i][j].second[0]);
            }
        }
    }
    // for (size_t i = 0; i < this->host.size(); i++)
    // {
    //     std::cout << "host " << this->host[i] << std::endl;
    // }
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
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1].c_str(), 0, splittedUri[1].length()) != std::string::npos)
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
        for (size_t i = 0; i < this->servers[portIndex].size(); i++)
        {
            if (this->servers[portIndex][i].second[0] == "/")
            {
                size_t j = i + 1;
                while(j < this->servers[portIndex].size() && this->servers[portIndex][j].first != "server.location")
                {
                    if (j < this->servers[portIndex].size() && this->servers[portIndex][j].first == "server_name")
                    {
                        this->serverName = this->servers[portIndex][j].second[0];
                        return;
                    }
                    j++;
                }
            }
        }
        if (this->serverName.size() == 0)
        {
            setServerName(0, "", port);
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
    std::cout << "port Index " << portIndex << std::endl;
    // std::cout << "isLocation " << isLocation << "    " << uri << " isLocation\n";
    if (!isLocation)
    {
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "root")
            {
                this->root = this->servers[portIndex][j].second[0];
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
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1].c_str(), 0, splittedUri[1].length()) != std::string::npos)
            {
                size_t k = j + 1;
                while(k < this->servers[portIndex].size() && this->servers[portIndex][k].first != "server.location")
                {
                    if (k < this->servers[portIndex].size() && this->servers[portIndex][k].first == "root")
                    {
                        this->root = this->servers[portIndex][k].second[0];
                        return;
                    }
                    k++;
                }
            }
        }
        for (size_t i = 0; i < this->servers[portIndex].size(); i++)
        {
            if (this->servers[portIndex][i].second[0] == "/")
            {
                size_t j = i + 1;
                while(j < this->servers[portIndex].size() && this->servers[portIndex][j].first != "server.location")
                {
                    if (j < this->servers[portIndex].size() && this->servers[portIndex][j].first == "root")
                    {
                        this->root = this->servers[portIndex][j].second[0];
                        return;
                    }
                    j++;
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
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1].c_str(), 0, splittedUri[1].length()) != std::string::npos)
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
        for (size_t i = 0; i < this->servers[portIndex].size(); i++)
        {
            if (this->servers[portIndex][i].second[0] == "/")
            {
                size_t j = i + 1;
                while(j < this->servers[portIndex].size() && this->servers[portIndex][j].first != "server.location")
                {
                    if (j < this->servers[portIndex].size() && this->servers[portIndex][j].first == "index")
                    {
                        this->index = this->servers[portIndex][j].second[0];
                        return;
                    }
                    j++;
                }
            }
        }
        if (this->index.size() == 0)
        {
            setIndex(0, "", port);
        }
    }
}

void Config::defineErrorPage(int isLocation, const std::string &uri, const std::string& port)
{
    (void)uri;
    this->isErrorPage.clear();
    size_t portIndex = getPortIndex(port);

    if (!isLocation)
    {
        for (size_t i = 0; i < this->servers[portIndex].size(); i++)
        {
            if (this->servers[portIndex][i].first == "server.location")
                return;
            if (this->servers[portIndex][i].first == "error_page")
            {
                // std::cout << "this->servers[portIndex][i].second[1] " << this->servers[portIndex][i].second[1] << std::endl;
                this->isErrorPage[this->servers[portIndex][i].second[0]] = this->servers[portIndex][i].second[1];
            }
        }
    }
    else
    {
        std::vector<std::string> splittedUri = split(uri, "/");
        splittedUri[1] = '/' + splittedUri[1] + '/';
        for (size_t j = 0; j < this->servers[portIndex].size(); j++)
        {
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1].c_str(), 0, splittedUri[1].length()) != std::string::npos)
            {
                size_t k = j + 1;
                while(k < this->servers[portIndex].size() && this->servers[portIndex][k].first != "server.location")
                {
                    if (k < this->servers[portIndex].size() && this->servers[portIndex][k].first == "error_page")
                    {
                        // std::cout << "this->servers[portIndex][k].second[1] " << this->servers[portIndex][k].second[1] << std::endl;
                        this->isErrorPage[this->servers[portIndex][k].second[0]] = this->servers[portIndex][k].second[1];
                        // return;
                    }
                    k++;
                }
            }
        }
        for (size_t i = 0; i < this->servers[portIndex].size(); i++)
        {
            if (this->servers[portIndex][i].second[0] == "/")
            {
                size_t j = i + 1;
                while(j < this->servers[portIndex].size() && this->servers[portIndex][j].first != "server.location")
                {
                    if (j < this->servers[portIndex].size() && this->servers[portIndex][j].first == "error_page")
                    {
                        // std::cout << "this->servers[portIndex][j].second[1] " << this->servers[portIndex][j].second[1] << std::endl;
                        this->isErrorPage[this->servers[portIndex][j].second[0]] = this->servers[portIndex][i].second[1];
                        // return;
                    }
                    j++;
                }
            }
        }
        if (this->isErrorPage.size() == 0)
        {
            defineErrorPage(0, "", port);
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
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1].c_str(), 0, splittedUri[1].length()) != std::string::npos)
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
        for (size_t i = 0; i < this->servers[portIndex].size(); i++)
        {
            if (this->servers[portIndex][i].second[0] == "/")
            {
                size_t j = i + 1;
                while(j < this->servers[portIndex].size() && this->servers[portIndex][j].first != "server.location")
                {
                    if (j < this->servers[portIndex].size() && this->servers[portIndex][j].first == "error_page")
                    {
                        this->errorPage = this->servers[portIndex][j].second;
                        return;
                    }
                    j++;
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
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1].c_str(), 0, splittedUri[1].length()) != std::string::npos)
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
        for (size_t i = 0; i < this->servers[portIndex].size(); i++)
        {
            if (this->servers[portIndex][i].second[0] == "/")
            {
                size_t j = i + 1;
                while(j < this->servers[portIndex].size() && this->servers[portIndex][j].first != "server.location")
                {
                    if (j < this->servers[portIndex].size() && this->servers[portIndex][j].first == "autoindex")
                    {
                        this->autoIndex = this->servers[portIndex][j].second[0];
                        return;
                    }
                    j++;
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
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1].c_str(), 0, splittedUri[1].length()) != std::string::npos)
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
        for (size_t i = 0; i < this->servers[portIndex].size(); i++)
        {
            if (this->servers[portIndex][i].second[0] == "/")
            {
                size_t j = i + 1;
                while(j < this->servers[portIndex].size() && this->servers[portIndex][j].first != "server.location")
                {
                    if (j < this->servers[portIndex].size() && this->servers[portIndex][j].first == "client_max_body_size")
                    {
                        this->clientMaxBodySize = this->servers[portIndex][j].second[0];
                        return;
                    }
                    j++;
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
            if (this->servers[portIndex][j].first == "server.location" && this->servers[portIndex][j].second[0].find(splittedUri[1].c_str(), 0, splittedUri[1].length()) != std::string::npos)
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
        for (size_t i = 0; i < this->servers[portIndex].size(); i++)
        {
            if (this->servers[portIndex][i].second[0] == "/")
            {
                size_t j = i + 1;
                while(j < this->servers[portIndex].size() && this->servers[portIndex][j].first != "server.location")
                {
                    if (j < this->servers[portIndex].size() && this->servers[portIndex][j].first == "upload_dir")
                    {
                        this->uploadDir = this->servers[portIndex][j].second[0];
                        return;
                    }
                    j++;
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

const std::string Config::getHost(int port)
{
    std::stringstream ss;
    ss << port;
    std::string portStr = ss.str();
    int portIndex = getPortIndex(portStr);
    
    for (size_t i = 0; i < this->servers[portIndex].size(); i++)
    {
        if (this->servers[portIndex][i].first == "host")
        {
            return this->servers[portIndex][i].second[0];
        }
    }
    
    return "";
}

void Config::setPortMap(int port)
{
    std::stringstream ss;
    ss << port;
    std::string portStr = ss.str();
    for (size_t i = 0; i < this->servers.size(); i++)
    {
        for (size_t j = 0; j < this->servers[i].size(); j++)
        {
            for (size_t p = 0; p < this->servers[i][j].second.size(); p++)
            {
                if (this->servers[i][j].second[p] == portStr)
                {
                    this->portMap[port] = i;
                    return;
                }
            }
        }
    }
}

std::map<int, std::string> Config::getPortMap()
{
    return this->portMap;
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

const std::string Config::getUploadDir()
{
    return this->uploadDir;
}

const std::map<std::string, int> Config::getAllowedMethods()
{
    return this->allowedMethods;
}

const std::map<std::string, std::string> Config::getIsErrorPage()
{
    return this->isErrorPage;
}

const std::vector<std::string> Config::getRedirect()
{
    return this->redirect;
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
    this->allowedMethods.clear();

    size_t portIndex = getPortIndex(port);
    if (!isLocation)
    {
        for (size_t i = 0; i < this->servers[portIndex].size(); i++)
        {
            if (this->servers[portIndex][i].first == "allowed_methods")
            {
                for (size_t j = 0; j < this->servers[portIndex][i].second.size(); j++)
                    this->allowedMethods[this->servers[portIndex][i].second[j]] = 1;
                return;
            }
        }
    }
    else
    {
        std::vector<std::string> splittedUri = split(uri, "/");
        splittedUri[1] = '/' + splittedUri[1] + '/';
        for (size_t i = 0; i < this->servers[portIndex].size(); i++)
        {
            if (this->servers[portIndex][i].first == "server.location" && this->servers[portIndex][i].second[0].find(splittedUri[1].c_str(), 0, splittedUri[1].length()) != std::string::npos)
            {
                size_t k = i + 1;
                while(k < this->servers[portIndex].size() && this->servers[portIndex][k].first != "server.location")
                {
                    if (k < this->servers[portIndex].size() && this->servers[portIndex][k].first == "allowed_methods")
                    {
                        for (size_t j = 0; j < this->servers[portIndex][k].second.size(); j++)
                            this->allowedMethods[this->servers[portIndex][k].second[j]] = 1;
                        return;
                    }
                    k++;
                }
            }
        }
        for (size_t i = 0; i < this->servers[portIndex].size(); i++)
        {
            if (this->servers[portIndex][i].second[0] == "/")
            {
                size_t k = i + 1;
                while(k < this->servers[portIndex].size() && this->servers[portIndex][k].first != "server.location")
                {
                    if (k < this->servers[portIndex].size() && this->servers[portIndex][k].first == "allowed_methods")
                    {
                        for (size_t j = k; j < this->servers[portIndex][k].second.size(); j++)
                            this->allowedMethods[this->servers[portIndex][k].second[j]] = 1;
                        return;
                    }
                    k++;
                }
            }
        }
        if (this->allowedMethods.size() == 0)
        {
            setAllowedMethods(0, "", port);
        }
    }
}

int Config::isLocation(const std::string &uri, const std::string& port)
{
    size_t index = getPortIndex(port);
    std::vector<std::string> splittedUri = split(uri, "/");
    splittedUri[1] = '/' + splittedUri[1] + '/';
    splittedUri[1] = normalizeUrl(splittedUri[1]);
    // std::cout << "splittedUri[1] " << splittedUri[1] << "   " << normalizeUrl(uri) << std::endl;
    for (size_t j = 0; j < this->servers[index].size(); j++)
    {
        if (this->servers[index][j].first == "server.location" && this->servers[index][j].second[0] == "/")
            return 1;
        if (this->servers[index][j].first == "server.location" && normalizeUrl(this->servers[index][j].second[0]) == splittedUri[1])
            return 1;
        if (splittedUri[1] != "/" && this->servers[index][j].first == "server.location" && this->servers[index][j].second[0].find(splittedUri[1].c_str(), 0, splittedUri[1].length()) != std::string::npos)
            return 1;
    }
    return 0;
}
