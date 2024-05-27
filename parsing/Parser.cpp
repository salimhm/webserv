/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 22:50:03 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/27 15:49:16 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "../utils.hpp"

Parser::Parser(){}

Parser::~Parser(){}

Parser::Parser(const std::string &filePath) : filePath(filePath)
{
    // std::cout << filePath << std::endl;
    this->allowedDirectives.push_back("listen"); //2
    this->allowedDirectives.push_back("server_name"); //2
    this->allowedDirectives.push_back("root"); //2
    this->allowedDirectives.push_back("index"); //2
    this->allowedDirectives.push_back("error_page"); //3
    this->allowedDirectives.push_back("autoindex"); //2
    this->allowedDirectives.push_back("client_max_body_size"); //2
    this->allowedDirectives.push_back("upload_dir"); //2
    this->allowedDirectives.push_back("allowed_methods"); //1-4
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

void Parser::setServerDirectives(std::vector<std::pair<std::string, std::vector<std::string> > > directives)
{
    size_t i = 0;
    for (; i < directives.size(); i++)
    {
        if (directives[i].first == "server.location")
            break;
        this->globalDirectives.push_back(std::make_pair(directives[i].first, directives[i].second));
    }
    size_t j;
    for (; i < directives.size(); i++)
    {
        j = i;
        if (directives[j].first == "server.location")
        {
            std::string path = directives[j].second[0];
            path = normalizeUrl(path);
            j++;
            while (j < directives.size() && directives[j].first != "server.location")
            {
                this->locations.push_back(std::make_pair(path, std::make_pair(directives[j].first, directives[j].second)));
                j++;
            }
        }
    }
}

int Parser::checkSyntax(std::vector<std::pair<std::string, std::vector<std::string> > > directives)
{
    int listenFound = 0;
    for (size_t i = 0; i < directives.size(); i++)
    {
        if (directives[i].first == "listen")
        {
            listenFound++;
            break;
        }
    }
    if (!listenFound)
        return 1;
    for (size_t i = 0; i < directives.size(); i++)
    {
        for (size_t j = 0; j < this->allowedDirectives.size(); j++)
        {
            if (directives[i].first == this->allowedDirectives[j] || directives[i].first == "server.location")
                break;
            else if (directives[i].first != this->allowedDirectives[j] && j == this->allowedDirectives.size() - 1)
            {
                return 1;
            }
        }
        if (directives[i].first == "listen")
        {
            if (directives[i].second.size() < 1 || directives[i].second.size() > 1)
                return 1;
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
        }
        if (directives[i].first == "upload_dir")
        {
            if (directives[i].second.size() < 1 || directives[i].second.size() > 1)
                return 1;
        }
    }
    return 0;
}

std::vector<std::pair<std::string, std::vector<std::string> > > Parser::parseServer(std::string &serverBlock)
{
    std::vector<std::pair<std::string, std::vector<std::string> > > serverDirectives;
    std::vector<std::pair<std::string, std::pair<std::string, std::vector<std::string> > > > locations;

    serverBlock = trim(serverBlock);
    // std::cout << "SERVER BLOCK " << std::endl;
    // std::cout << serverBlock << std::endl;
    std::vector<std::string> serverBlocks = split(serverBlock, "\n");
    size_t i = 0;
    while (serverBlocks.size())
    {
        std::vector<std::string> directives = split(serverBlocks[i], " ");
        if (directives.size() >= 2)
        {
            std::string directive = directives[0];
            directives.erase(directives.begin());
            serverDirectives.push_back(std::make_pair(directive, directives));
        }
        else if (directives.size() < 2 && directives[0].size() != 0)
        {
            // std::cout << directives[0].size() << std::endl;
            throw std::runtime_error("Syntax error!");
        }
        serverBlocks.erase(serverBlocks.begin());
        std::vector<std::string> serverBlocks = split(serverBlock, "\n");
    }
    setServerDirectives(serverDirectives);
    return serverDirectives;
}

void Parser::parse()
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open config file");
    }

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
        if (serverBlocks[0] == "server" || serverBlocks[0] == "server:")
        {
            file.seekg(serverPos);
            getline(file, line);
            std::string server = getServers(file);
            servers.push_back(server);
        }
    }
    std::cout << "SIZE " << servers.size() << std::endl;
    for (size_t i = 0; i < servers.size(); i++)
    {
        // std::cout << servers[i] << std::endl;
        if (!servers[i].size())
            throw std::runtime_error("Syntax error!");
        serverDirectives = parseServer(servers[i]);
        allServers.push_back(serverDirectives);
        if (checkSyntax(serverDirectives) == 1)
            throw std::runtime_error("Syntax error!");        
    }
}
