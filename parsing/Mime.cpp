/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:31:26 by shmimi            #+#    #+#             */
/*   Updated: 2024/06/09 01:04:35 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mime.hpp"

Mime::Mime() : _file("./assets/mime.types")
{
    std::ifstream file("./assets/mime.types");
    if (!file.is_open())
        throw std::runtime_error("Error: could not open mime.types file!");
    std::string line;
    std::vector<std::string> keys;
    std::vector<std::string> subKeys;
    std::map<std::vector<std::string>, std::string> mimeMap;
    while (getline(this->_file, line))
    {
        keys = split(line, "|");
        subKeys = split(keys[0], ", ");
        std::vector<std::string> keyVector;
        for (size_t i = 0; i < subKeys.size(); i++)
        {
            size_t counter = 0;
            while (counter != subKeys.size())
            {
                keyVector.push_back(subKeys[counter]);
                counter++;
            }
            mimeMap[keyVector] = keys[1];
            break;
        }
    }
    file.close();
    this->_mimeMap = mimeMap;
}

Mime::~Mime()
{
    this->_file.close();
}

const std::ifstream &Mime::getFile() const
{
    return this->_file;
}

const std::string Mime::getContentType(const std::string &extension) const
{
    std::map<std::vector<std::string>, std::string>::const_iterator it = this->_mimeMap.begin();
    while (it != this->_mimeMap.end())
    {
        for (size_t i = 0; i < it->first.size(); i++)
        {
            if (it->first[i] == extension)
            {
                // std::cout << it->first[i] << " ==> " << it->second << std::endl;
                return it->second;
            }
        }
        *it++;
    }
    return "";
}

const std::map<std::vector<std::string>, std::string> &Mime::getMimeMap() const
{
    return this->_mimeMap;
}
