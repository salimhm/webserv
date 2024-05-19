/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:27:33 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/15 21:40:31 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server/Server.hpp"
#include "../utils.hpp"

class Mime
{
    private:
        std::ifstream _file;
        std::map<std::vector<std::string>, std::string> _mimeMap;
    public:
        Mime();
        const std::ifstream& getFile() const;
        const std::string getContentType(const std::string& extension) const;
        // const std::string& getFileContent() const;
        const std::map<std::vector<std::string>, std::string>& getMimeMap() const;
};

