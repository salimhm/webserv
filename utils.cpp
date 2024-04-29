/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:31:37 by shmimi            #+#    #+#             */
/*   Updated: 2024/04/29 15:33:50 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<std::string> split(const std::string &toSplit, std::string delimiter)
{
    std::vector<std::string> splitted;
    if (delimiter.size() == 1)
    {
        for (size_t i = 0; i < toSplit.size(); i++)
        {
            if (toSplit[i] == delimiter[0])
                splitted.push_back(toSplit.substr(0, i));
            if (i == toSplit.size() - 1)
                splitted.push_back(toSplit.substr(0, i + 1));
        }
    }
    else
    {
        std::string copy = toSplit;
        size_t pos = copy.find(delimiter);
        while (pos != std::string::npos)
        {
            splitted.push_back(copy.substr(0, pos));
            copy.erase(0, pos + delimiter.size());
            pos = copy.find(delimiter);            
        }
        splitted.push_back(copy);
    }
    return splitted;
}
