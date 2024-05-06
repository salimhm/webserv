/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:15:53 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/05 17:32:39 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server/Server.hpp"
#include "server/Mime.hpp"

class Config: public Mime
{
    private:
        // std::string serverName;
        // std::string root;
        // std::vector<std::string> index;
        // std::map<int, std::string> errorPage;
        // std::string autoIndex;
        // std::string _clientMaxBodySize;
    public:
        Config();
        Config(const Config& cpy);
        Config& operator=(const Config& cpy);
        const std::vector<int> getPort() const;
        const std::string getServerName() const;
        const std::string getRoot() const;
        const std::map<int, std::string> getErrorPage() const;
        const std::vector<std::string> getIndex() const;
        const std::string getAutoIndex() const;
        int getClientMaxBodySize() const;
};