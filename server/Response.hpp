/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 13:24:47 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/01 16:42:08 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "../client/Client.hpp"

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