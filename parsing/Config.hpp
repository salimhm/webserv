/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:15:53 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/16 20:24:17 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server/Server.hpp"
#include "../parsing/Mime.hpp"

class Config: public Mime
{
    private:
        // std::string serverName;
        // std::string root;
        // std::vector<std::string> index;
        // std::map<int, std::string> errorPage;
        // std::string autoIndex;
        // std::string _clientMaxBodySize;
        std::string& filePath;
        static std::string nullFile; //Contains nothing
    public:
        Config();
        Config(std::string& file);
        Config(const Config& cpy);
        Config& operator=(const Config& cpy);

        // std::string getFileContent(const std::string& filename) const;

        const std::vector<int> getPort() const;
        const std::string getServerName() const;
        const std::string getRoot() const;
        const std::map<int, std::string> getErrorPage() const;
        const std::vector<std::string> getIndex() const;
        const std::string getAutoIndex() const;
        int getClientMaxBodySize() const;

        void resetFile() const;
};