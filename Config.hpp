/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:15:53 by shmimi            #+#    #+#             */
/*   Updated: 2024/04/23 23:43:22 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

class Config
{
    public:
        int port;
        std::string serverName;
        std::string root;
        std::vector<std::string> index;

        std::vector<int> getPort();
        std::string getServerName();
        std::string getRoot();
        // std::vector<std::string> getIndex();
};