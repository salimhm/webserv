/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:19:25 by shmimi            #+#    #+#             */
/*   Updated: 2024/04/23 23:43:35 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include <sstream>

std::vector<int> Config::getPort()
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

std::string Config::getServerName()
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

std::string Config::getRoot()
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

// std::vector<std::string> Config::getIndex()
// {
//     std::ifstream config("webserv.conf");
//     std::vector<std::string> index;
//     std::string line;
//     std::string root;
//     while (getline(config, line))
//     {

//         if (line.find("index") != std::string::npos)
//         {
//             // std::cout << line << std::endl;
//             int pos = line.find(";");
//             root = line.substr(line.find(" ") + 1, pos - line.find(" ") - 1);
//             std::cout << root << std::endl;
//         }
//     }
//     return index;
// }