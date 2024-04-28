/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 02:36:33 by shmimi            #+#    #+#             */
/*   Updated: 2024/04/28 06:25:11 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

class Client
{
    private:
        int port;
        int clientFd;
        std::vector<std::string> request;
        
    public:
        Client(int port, int clientFd, std::vector<std::string> request);
        /********** Getters *************/
        int getClientFd() const;
        int getPort() const;
        const std::vector<std::string> &getRequest() const;
        
};