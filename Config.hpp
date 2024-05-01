/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:15:53 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/01 01:26:35 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server/Server.hpp"

class Config
{
    private:
        std::string serverName;
        std::string root;
        std::vector<std::string> index;
        
    public:

        std::vector<int> getPort();
        std::string getServerName();
        std::string getRoot();
        // std::vector<std::string> getIndex();
};