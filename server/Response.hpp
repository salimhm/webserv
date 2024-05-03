/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 13:24:47 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/03 15:59:58 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "../client/Client.hpp"

enum StatusCode
{
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    BAD_REQUEST = 400,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    REQUEST_TIMEOUT = 408,
    PAYLOAD_TOO_LARGE = 413,
    URI_TOO_LONG = 414,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
};

class Response
{
    private:
        std::string _status;
        std::string _statusMessage;
        std::string _body;
        std::string _headers;
        std::string _httpVersion;
        std::string _contentType;
        std::string _contentLength;

    public:
        /********** Getters **********/
        const std::string& getHttpVersion() const;
        const std::string& getStatus() const;
        const std::string& getStatusMessage() const;
        const std::string& getHeaders() const;
        const std::string& getBody() const;
        const std::string& getContentType() const;
        const std::string& getContentLength() const;

        /********** Setters **********/
        void setHttpVersion(const std::string& httpVersion);
        void setStatus(const std::string& status);
        void setStatusMessage(const std::string& statusMessage);
        void setHeaders(const std::string& headers);
        void setBody(const std::string& body);
        void setContentType(const std::string& contentType);
        void setContentLength(const std::string& contentLength);
};