/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 00:04:21 by shmimi            #+#    #+#             */
/*   Updated: 2024/06/09 16:13:29 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(const int clientFd, const std::string& filePath) : Config(filePath), clientFd(clientFd)
{
    _totalBytes = 0;
    _bytesToSend = 0;
    _bytesSent = 0;
    headersParsed = 0;
    

}

// ******************************* Getters *******************************
int Client::getClientFd() const
{
    return this->clientFd;
}

const std::vector<std::string>& Client::getRequest() const
{
    return startLine;
}

const std::vector< std::pair<std::string, std::string> >& Client::getHeaders() const
{
    return this->headers;
}

const std::map<std::string, std::string>& Client::getHeadersmap() const
{
    return headers_map;
}

const std::string& Client::getBody() const
{
    return this->body;
}

const std::string& Client::getMethod() const
{
    return this->startLine[0];
}

const std::string& Client::getUri() const
{
    return this->startLine[1];
}

const std::string& Client::getVersion() const
{
    return this->startLine[2];
}

const std::string& Client::getPort() const
{
    return this->port;
}


const int& Client::getTotalBytes()
{
    return this->_totalBytes;
}

const int& Client::getBytesToSend()
{
    return this->_bytesToSend;
}

const int& Client::getBytesSent()
{
    return this->_bytesSent;
}
// ******************************* Setters *******************************
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

void Client::setTotalBytes(const int &totalBytes)
{
    this->_totalBytes = totalBytes;
}

void Client::setBytesToSend(const int& bytesToSend)
{
    this->_bytesToSend = bytesToSend;
}

void Client::setBytesSent(const int& bytesSent)
{
    this->_bytesSent = bytesSent;
}

void Client::setBody(const std::string& body)
{
    // this->body = body;
    this->body.append(body);
}

void Client::setPort(const std::string& port)
{
    this->port = port;
}

void Client::addStartLine(const std::string& line)
{
    this->startLine.push_back(line);
}

void Client::setHeaders(const std::vector< std::pair<std::string, std::string> >& headers)
{
    this->headers = headers;
}

void Client::setHeadersMap(const std::map <std::string, std::string>& headers_map)
{
    this->headers_map = headers_map;
}

void Client::setCrLf(int crlf)
{
    this->crlf = crlf;
}

int Client::getCrLf()
{
    return this->crlf;
}

void Client::setBytesRead(const std::string& bytesRead)
{
    this->bytesRead.append(bytesRead);
}

std::string& Client::getBytesRead()
{
    return this->bytesRead;
}

void Client::setisHeaderParser()
{
    this->headersParsed = 1;
}


int Client::getIsHeaderParser()
{
    return this->headersParsed;
}


void Client::setAllRequest(std::string& allRequest)
{
    this->allRequest.append(allRequest);
}

std::string Client::getAllRequest()
{
    return this->allRequest;
}