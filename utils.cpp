/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:31:37 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/03 18:51:26 by shmimi           ###   ########.fr       */
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
