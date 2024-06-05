/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:31:37 by shmimi            #+#    #+#             */
/*   Updated: 2024/06/05 20:57:27 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"

std::string trim(const std::string &str)
{
    size_t i = 0;
    std::string copy = str;
    if (str[0] == ' ' || str[i] == '\t')
    {
        while (isspace(str[i]) || str[i] == '\t')
            i++;
    }
    copy.erase(0, i);
    for (; i < copy.size(); i++)
    {
        if (copy[i] == '\n')
        {
            i++;
            while (isspace(copy[i]) || copy[i] == '\t')
                copy.erase(i, 1);
        }
    }
    for (size_t i = 0; i < copy.size(); i++)
    {
        if (copy[i] == '\t')
            copy[i] = ' ';
    }
    for (size_t i = 0; i < copy.size(); i++)
    {
        if (copy[i] == '\t')
            copy[i] = ' ';
        if (isspace(copy[i]) || copy[i] == '\t')
        {
            size_t firstSpace = i;
            size_t countSpace = 0;
            while (isspace(copy[i]) || copy[i] == '\t')
            {
                countSpace++;
                i++;
            }
            if (countSpace > 1)
                copy.erase(firstSpace, countSpace - 1);
        }
    }
    return copy;
}

std::vector<std::string> split(const std::string &toSplit, std::string delimiter)
{
    std::vector<std::string> splitted;
    std::string copy = trim(toSplit);
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