/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 00:04:21 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/28 15:58:20 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(const int clientFd, const std::string& filePath) : Config(filePath), clientFd(clientFd) {}


int Client::getClientFd() const
{
    return this->clientFd;
}

const std::vector<std::string>& Client::getRequest() const
{
    return request.startLine;
}

const std::vector< std::pair<std::string, std::string> >& Client::getHeaders() const
{
    return request.headers;
}

const std::string& Client::getBody() const
{
    return request.body;
}

const std::string& Client::getMethod() const
{
    return this->request.startLine[0];
}

const std::string& Client::getUri() const
{
    return this->request.startLine[1];
}

const std::string& Client::getVersion() const
{
    return this->request.startLine[2];
}

const std::string& Client::getPort() const
{
    return this->request.port;
}

void Client::setMethod(const std::string &method)
{
    this->_method = method;
}

void Client::setUri(const std::string &uri)
{
    this->_uri = uri;
}

void Client::setVersion(const std::string &version)
{
    this->_version = version;
}

void Client::setRequest(const Request &request)
{
    this->request = request;
}