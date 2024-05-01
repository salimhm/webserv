/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 14:16:57 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/01 16:42:14 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

const std::string& Response::getStatus() const
{
    return this->_status;
}

const std::string& Response::getStatusMessage() const
{
    return this->_statusMessage;
}

const std::string& Response::getBody() const
{
    return this->_body;
}

const std::string& Response::getHeaders() const
{
    return this->_headers;
}

const std::string& Response::getHttpVersion() const
{
    return this->_httpVersion;
}

const std::string& Response::getContentType() const
{
    return this->_contentType;
}

const std::string& Response::getContentLength() const
{
    return this->_contentLength;
}

void Response::setStatus(const std::string& status)
{
    this->_status = status;
}

void Response::setStatusMessage(const std::string& statusMessage)
{
    this->_statusMessage = statusMessage;
}

void Response::setBody(const std::string& body)
{
    this->_body = body;
}

void Response::setHeaders(const std::string& headers)
{
    this->_headers = headers;
}

void Response::setHttpVersion(const std::string& httpVersion)
{
    this->_httpVersion = httpVersion;
}

void Response::setContentType(const std::string& contentType)
{
    this->_contentType = contentType;
}

void Response::setContentLength(const std::string& contentLength)
{
    this->_contentLength = contentLength;
}
