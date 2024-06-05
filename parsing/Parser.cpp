/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 22:50:03 by shmimi            #+#    #+#             */
/*   Updated: 2024/06/05 20:58:28 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "../utils.hpp"

Parser::Parser(){}

Parser::~Parser(){}

Parser::Parser(const std::string &filePath) : filePath(filePath)
{
    this->allowedDirectives.push_back("listen"); //2
    this->allowedDirectives.push_back("server_name"); //2
    this->allowedDirectives.push_back("root"); //2
    this->allowedDirectives.push_back("index"); //2
    this->allowedDirectives.push_back("error_page"); //3
    this->allowedDirectives.push_back("autoindex"); //2
    this->allowedDirectives.push_back("client_max_body_size"); //2
    this->allowedDirectives.push_back("upload_dir"); //2
    this->allowedDirectives.push_back("allowed_methods"); //1-4
    this->allowedDirectives.push_back("redirect"); //1-4
}

std::string getServers(std::ifstream &file)
{
    std::string line;
    std::string block;
    std::vector<std::string> serverBlocks;
    std::streampos serverPos;
    while (1)
    {
        serverPos = file.tellg();
        if (!getline(file, line))
            break;
        serverBlocks = split(line, " ");
        for (size_t i = 0; i < serverBlocks.size(); i++)
        {
            if (serverBlocks[i] == "server" || serverBlocks[i] == "server:")
            {
                file.seekg(serverPos);
                return block;
            }
        }
        block += line + "\n";
    }
    return block;
}

std::vector<std::pair<std::string, std::pair<std::string, std::vector<std::string> > > > Parser::getLocations()
{
    return this->locations;
}

std::vector<std::pair<std::string, std::vector<std::string> > > Parser::getGlobalDirectives()
{
    return this->globalDirectives;
}

std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > > Parser::getAllServers()
{
    return this->allServers;
}

void Parser::setServerDirectives(std::vector<std::pair<std::string, std::vector<std::string> > >& directives)
{
    std::vector<std::string> keys;
    size_t i = 0;
    for (; i < directives.size(); i++)
    {
        if (directives[i].first == "server.location")
            break;
        this->globalDirectives.push_back(std::make_pair(directives[i].first, directives[i].second));
        keys.push_back(directives[i].first);
    }
    checkGlobalDuplicates(keys);
    keys.clear();
    size_t j;
    for (; i < directives.size(); i++)
    {
        j = i;
        if (directives[j].first == "server.location")
        {
            std::string path = directives[j].second[0];
            size_t pos = path.find(":", 0);
            if (pos != std::string::npos)
                path = path.substr(0, pos);
            path = normalizeUrl(path);
            j++;
            while (j < directives.size() && directives[j].first != "server.location")
            {
                this->locations.push_back(std::make_pair(path, std::make_pair(directives[j].first, directives[j].second)));
                keys.push_back(directives[j].first);
                j++;
            }
            checkLocationsDuplicates(keys);
            keys.clear();
        }
    }
}

int Parser::checkSyntax(std::vector<std::pair<std::string, std::vector<std::string> > > directives)
{
    for (size_t i = 0; i < directives.size(); i++)
    {
        for (size_t j = 0; j < this->allowedDirectives.size(); j++)
        {
            if (directives[i].first == this->allowedDirectives[j] || directives[i].first == "server.location")
                break;
            else if (directives[i].first != this->allowedDirectives[j] && j == this->allowedDirectives.size() - 1)
                return 1;
        }
        if (directives[i].first == "listen")
        {
            // if (directives[i].second.size() < 1 || directives[i].second.size() > 1)
            //     return 1;
            int port;
            std::stringstream num;
            std::istringstream(directives[i].second[0]) >> port;
            num << port;
            if (directives[i].second[0].size() != num.str().size())
                return 1;
        }
        if (directives[i].first == "server_name")
        {
            if (directives[i].second.size() < 1 || directives[i].second.size() > 1)
                return 1;
        }
        if (directives[i].first == "autoindex")
        {
            if (directives[i].second.size() < 1 || directives[i].second.size() > 1)
                return 1;
        }
        if (directives[i].first == "client_max_body_size")
        {
            if (directives[i].second.size() < 1 || directives[i].second.size() > 1)
                return 1;
        }
        if (directives[i].first == "root")
        {
            if (directives[i].second.size() < 1 || directives[i].second.size() > 1)
                return 1;
        }
        if (directives[i].first == "index")
        {
            if (directives[i].second.size() < 1 || directives[i].second.size() > 1)
                return 1;
        }
        if (directives[i].first == "error_page")
        {
            if (directives[i].second.size() < 2 || directives[i].second.size() > 2)
                return 1;
        }
        if (directives[i].first == "autoindex")
        {
            if (directives[i].second.size() < 1 || directives[i].second.size() > 1)
                return 1;
        }
        if (directives[i].first == "allowed_methods")
        {
            if (directives[i].second.size() < 1 || directives[i].second.size() > 3)
                return 1;
            for (size_t j = 0; j < directives[i].second.size(); j++)
            {
                if (directives[i].second[j] != "GET" && directives[i].second[j] != "POST" && directives[i].second[j] != "DELETE")
                    return 1;
            }
        }
        if (directives[i].first == "upload_dir")
        {
            if (directives[i].second.size() < 1 || directives[i].second.size() > 1)
                return 1;
        }
        if (directives[i].first == "client_max_body_size")
        {
            if (directives[i].second.size() < 1 || directives[i].second.size() > 1)
                return 1;
        }
        if (directives[i].first == "redirect")
        {
            if (directives[i].second.size() < 2 || directives[i].second.size() > 2)
                return 1;
            if (directives[i].second[1][0] != '/' && directives[i].second[1].find("http://", 0, 7) != 0)
                return 1;
            else if (directives[i].second[1][0] != '/' && directives[i].second[1].size() < 8)
                return 1;
        }
    }
    return 0;
}

void Parser::checkGlobalDuplicates(const std::vector<std::string>& keys)
{
    int listen = 0;
    int autoIndex = 0;
    int serverName = 0;
    int root = 0;
    int index = 0;
    int allowedMethods = 0;
    int uploadDir = 0;
    int clientMaxBodySize = 0;
    int redirect = 0;
    for (size_t i = 0; i < keys.size(); i++)
    {
        if (keys[i] == "listen")
            listen++;
        if (keys[i] == "server_name")
            serverName++;
        if (keys[i] == "root")
            root++;
        if (keys[i] == "index")
            index++;
        if (keys[i] == "allowed_methods")
            allowedMethods++;
        if (keys[i] == "upload_dir")
            uploadDir++;
        if (keys[i] == "autoindex")
            autoIndex++;
        if (keys[i] == "client_max_body_size")
            clientMaxBodySize++;
        if (keys[i] == "redirect")
            redirect++;
    }
    if (listen > 1 || redirect > 1 || serverName > 1 || root > 1 || index > 1 || allowedMethods > 1 || uploadDir > 1 || autoIndex > 1 || clientMaxBodySize > 1)
        throw std::runtime_error("Syntax error!");
}

void Parser::checkLocationsDuplicates(const std::vector<std::string>& keys)
{
    int root = 0;
    int index = 0;
    int allowedMethods = 0;
    int uploadDir = 0;
    int redirect = 0;
    for (size_t i = 0; i < keys.size(); i++)
    {
        if (keys[i] == "listen")
            throw std::runtime_error("Syntax error!");
        if (keys[i] == "server_name")
            throw std::runtime_error("Syntax error!");
        if (keys[i] == "root")
            root++;
        if (keys[i] == "index")
            index++;
        if (keys[i] == "allowed_methods")
            allowedMethods++;
        if (keys[i] == "upload_dir")
            uploadDir++;
        if (keys[i] == "redirect")
            redirect++;
    }
    if (root > 1 || index > 1 || allowedMethods > 1 || uploadDir > 1 || redirect > 1)
        throw std::runtime_error("Syntax error!");
}

std::vector<std::pair<std::string, std::vector<std::string> > > Parser::parseServer(std::string &serverBlock)
{
    std::vector<std::pair<std::string, std::vector<std::string> > > serverDirectives;
    std::vector<std::pair<std::string, std::pair<std::string, std::vector<std::string> > > > locations;

    serverBlock = trim(serverBlock);
    std::vector<std::string> serverBlocks = split(serverBlock, "\n");
    size_t i = 0;
    while (serverBlocks.size())
    {
        std::vector<std::string> directives = split(serverBlocks[i], " ");
        // std::cout << "DIRECTIVES SIZE " << directives.size() << std::endl;
        if (directives[0] == "server.location")
        {
            if ((directives.size() == 2 && directives[1][directives[1].length() - 1] != ':') || (directives.size() == 3 && directives[2] != ":"))
                throw std::runtime_error("Syntax error!");
            else if (directives.size() == 2 && directives[1][directives[1].length() - 2] == ':')
                throw std::runtime_error("Syntax error!");
            else if (directives.size() == 3 && (directives[2].size() > 1 && directives[2][directives[2].length() - 2] == ':'))
                throw std::runtime_error("Syntax error!");
            else if (directives.size() == 3 && directives[1][directives[1].length() - 1] == ':' && directives[2] == ":")
                throw std::runtime_error("Syntax error!");
        }
        if (directives.size() >= 2)
        {
            std::string directive = directives[0];
            directives.erase(directives.begin());
            serverDirectives.push_back(std::make_pair(directive, directives));
        }
        else if (directives.size() < 2 && directives[0].size() != 0)
        {
            throw std::runtime_error("Syntax error!");
        }
        serverBlocks.erase(serverBlocks.begin());
        std::vector<std::string> serverBlocks = split(serverBlock, "\n");
    }
    setServerDirectives(serverDirectives);
    return serverDirectives;
}

int checkListen(const std::vector<std::string>& keys)
{
    for (size_t i = 0; i < keys.size(); i++)
    {
        if (keys[i] == "listen")
            return 1;
    }
    return 0;
}

void Parser::parse()
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open config file");
    }
    size_t port = 0;
    std::string line;
    std::string block;
    std::vector<std::string> serverBlocks;
    std::vector<std::string> servers;
    std::streampos serverPos;
    std::vector<std::pair<std::string, std::vector<std::string> > > serverDirectives;
    std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > > allServers;
    while (1)
    {
        serverPos = file.tellg();
        if (!getline(file, line))
            break;
        block += line;
        serverBlocks = split(line, " ");
        if ((serverBlocks[0] == "server:" && serverBlocks.size() == 2) || (serverBlocks[0] == "server" && serverBlocks.size() < 2))
            throw std::runtime_error("Syntax error!");
        else if (serverBlocks[0][serverBlocks[0].size() - 1] == ':' && serverBlocks[0][serverBlocks[0].size() - 2] == ':')
            throw std::runtime_error("Syntax error!");
        else if (serverBlocks.size() == 2 && (serverBlocks[1][serverBlocks[1].size() - 1] == ':' && serverBlocks[1][serverBlocks[1].size() - 2] == ':'))
            throw std::runtime_error("Syntax error!");
        else if (serverBlocks.size() > 2)
            throw std::runtime_error("Syntax error!");
        if (serverBlocks[0] == "server" || serverBlocks[0] == "server:")
        {
            file.seekg(serverPos);
            getline(file, line);
            std::string server = getServers(file);
            servers.push_back(server);
        }
    }
    std::cout << "SIZE " << servers.size() << std::endl;
    if (servers.size() == 0)
        throw std::runtime_error("Syntax error!");
    for (size_t i = 0; i < servers.size(); i++)
    {
        if (!servers[i].size())
            throw std::runtime_error("Syntax error!");
        serverDirectives = parseServer(servers[i]);
        allServers.push_back(serverDirectives);
        if (checkSyntax(serverDirectives) == 1 || port > 1)
            throw std::runtime_error("Syntax error!");
    }

    this->allServers = allServers;
 
    for (size_t i = 0; i < this->allServers.size(); i++)
    {
        for (size_t j = 0; j < this->allServers[i].size(); j++)
        {
            if (this->allServers[i][j].first == "listen")
                port++;
            if (this->allServers[i][j].first == "server.location")
            {
                size_t pos = this->allServers[i][j].second[0].find(":", 0);
                if (pos != std::string::npos)
                    this->allServers[i][j].second[0] = this->allServers[i][j].second[0].substr(0, pos);
                this->allServers[i][j].second[0] = normalizeUrl(this->allServers[i][j].second[0]);
            }
        }
    }   
    if (port == 0 || port != servers.size())
        throw std::runtime_error("Syntax error!");
}
