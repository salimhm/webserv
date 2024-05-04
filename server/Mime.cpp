/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:31:26 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/04 16:57:53 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mime.hpp"

Mime::Mime() : _file("mime.types")
{
    std::ifstream file("mime.types");
    if (!file.is_open())
    {
        std::cerr << "Error: could not open mime.types file" << std::endl;
        throw std::exception();
    }
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

    // std::map<std::vector<std::string>, std::string>::iterator it = mimeMap.begin();
    // while (it != mimeMap.end())
    // {
    //     if (it->first[1] == ".html")
    //         std::cout << it->first[0] << "==>" << it->second << std::endl;
    //     it++;
    // }
}

const std::ifstream &Mime::getFile() const
{
    return this->_file;
}

// const std::string& Mime::getFileContent() const
// {
//     return this->fileContent;
// }

// const std::map<std::vector<std::string>, std::string> Mime::getExtension()
// {
//     std::string line;
//     std::vector<std::string> keys;
//     std::vector<std::string> subKeys;
//     std::map<std::vector<std::string>, std::string> mimeMap;
//     while (getline(this->_file, line))
//     {
//         keys = split(line, "|");
//         subKeys = split(keys[0], ", ");
//         std::vector<std::string> keyVector;
//         for (size_t i = 0; i < subKeys.size(); i++)
//         {
//             size_t counter = 0;
//             while (counter != subKeys.size())
//             {
//                 keyVector.push_back(subKeys[counter]);
//                 counter++;
//             }
//             mimeMap[keyVector] = keys[1];
//         }
//     }
//     // std::map<std::vector<std::string>, std::string>::iterator it = mimeMap.begin();
//     // while (it != mimeMap.end())
//     // {
//     //     if (it->first[0] == ".aac")
//     //         std::cout << it->first[2] << "==>" << it->second << std::endl;
//     //     it++;
//     // }
//     this->_mimeMap = mimeMap;
//     return mimeMap;
// }

const std::string Mime::getContentType(const std::string &extension) const
{
    std::map<std::vector<std::string>, std::string>::const_iterator it = this->_mimeMap.begin();
    while (it != this->_mimeMap.end())
    {
        for (size_t i = 0; i < it->first.size(); i++)
        {
            if (it->first[i] == extension)
            {
                std::cout << it->second << std::endl;
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
