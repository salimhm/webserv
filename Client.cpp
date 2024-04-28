/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 00:04:21 by shmimi            #+#    #+#             */
/*   Updated: 2024/04/28 07:30:09 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int port, int clientFd, std::vector<std::string> request): port(port), clientFd(clientFd), request(request)
{
}


int Client::getClientFd() const
{
    return this->clientFd;
}

int Client::getPort() const
{
    return this->port;
}

const std::vector<std::string>& Client::getRequest() const
{
    return this->request;
}