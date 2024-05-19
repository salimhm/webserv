/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 22:50:03 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/19 17:43:59 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "../utils.hpp"

Parser::Parser(std::string &filePath) : filePath(filePath) {}

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
            if (serverBlocks[i] == "server")
            {
                file.seekg(serverPos);
                return block;
            }
        }
        block += line + "\n";
    }
    return block;
}

std::string trim(std::string &str)
{
    size_t i = 0;
    if (str[0] == ' ')
    {
        while (isspace(str[i]))
            i++;
    }
    str.erase(0, i);
    for (; i < str.size(); i++)
    {
        if (str[i] == '\n')
        {
            i++;
            while (isspace(str[i]))
                str.erase(i, 1);
        }
    }

    for (size_t i = 0; i < str.size(); i++)
    {
        if (isspace(str[i]))
        {
            int firstChar = i;
            int countSpace = 0;
            while (isspace(str[i]))
            {
                countSpace++;
                i++;
            }
            if (countSpace > 1)
                str.erase(firstChar, countSpace - 1);
        }
    }

    return str;
}

void parseServer(std::string &serverBlock)
{
    std::map<std::string, std::vector<std::string> > serverDirectives;

    serverBlock = trim(serverBlock);
    // std::cout << "SERVER BLOCK " << std::endl;
    // std::cout << serverBlock << std::endl;
    std::vector<std::string> serverBlocks = split(serverBlock, "\n");
    size_t i = 0;
    while (i < serverBlocks.size())
    {
        std::vector<std::string> directives = split(serverBlocks[i], " ");
        if (directives.size() >= 2)
            std::cout << "DIRECTIVE " << directives[0] << "==>" << directives[1] << std::endl;
        serverBlocks.erase(serverBlocks.begin());
        std::vector<std::string> serverBlocks = split(serverBlock, "\n");
    }
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
    while (1)
    {
        serverPos = file.tellg();
        if (!getline(file, line))
            break;
        block += line;
        serverBlocks = split(line, " ");
        if (serverBlocks[0] == "server")
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
        // std::cout << "====> SERVERRR " << i + 1 << " <====" << std::endl;
        // std::cout << servers[i] << std::endl;
        parseServer(servers[i]);
    }
}
