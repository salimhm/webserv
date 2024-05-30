/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:31:37 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/30 10:10:52 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"

std::vector<std::string> split(const std::string &toSplit, std::string delimiter)
{
    std::vector<std::string> splitted;
    std::string copy = toSplit;
    size_t pos = copy.find(delimiter);
    while (pos != std::string::npos)
    {
        splitted.push_back(copy.substr(0, pos));
        copy.erase(0, pos + delimiter.size());
        pos = copy.find(delimiter);
    }
    splitted.push_back(copy);
    return splitted;
}

std::string trim(std::string &str)
{
    size_t i = 0;
    if (str[0] == ' ' || str[i] == '\t')
    {
        while (isspace(str[i]) || str[i] == '\t')
            i++;
    }
    str.erase(0, i);
    for (; i < str.size(); i++)
    {
        if (str[i] == '\n')
        {
            i++;
            while (isspace(str[i]) || str[i] == '\t')
                str.erase(i, 1);
        }
    }
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == '\t')
            str[i] = ' ';
    }
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == '\t')
            str[i] = ' ';
        if (isspace(str[i]) || str[i] == '\t')
        {
            size_t firstSpace = i;
            size_t countSpace = 0;
            while (isspace(str[i]) || str[i] == '\t')
            {
                countSpace++;
                i++;
            }
            if (countSpace > 1)
                str.erase(firstSpace, countSpace - 1);
        }
    }

    return str;
}

std::string normalizeUrl(const std::string &uri)
{
    std::string newUri;
    if (uri.size() > 1)
    {
        int i = 0;
        while (uri[i])
        {
            if (uri[i] == '/')
            {
                while (uri[i] == '/')
                    i++;
                newUri += '/';
                continue;
            }
            else
                newUri += uri[i];
            i++;
        }
    }
    else
        return uri;
    if (newUri[newUri.size() - 1] != '/')
        newUri += '/';
    // std::cout << "newURI ========= " << newUri << " " << newUri.size() << std::endl;
    return newUri;
}