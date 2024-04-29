/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 00:04:21 by shmimi            #+#    #+#             */
/*   Updated: 2024/04/29 17:55:00 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(const int port, const int clientFd, const Request& request) : port(port), clientFd(clientFd), request(request) {}


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
    return request.startLine;
}

const std::map<std::string, std::string>& Client::getHeaders() const
{
    return request.headers;
}