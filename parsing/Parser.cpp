/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 22:50:03 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/27 12:28:44 by shmimi           ###   ########.fr       */
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
    this->allowedDirectives.push_back("allowed_methods"); //0-4
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
            // if (path[path.size() - 1] != '/')
            //     path[path.size() - 1] = '/';
            // else
            path = normalizeUrl(path);
            // std::cout << "PATH =>>>>>>>" << path << std::endl;
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
