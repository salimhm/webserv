/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 02:36:33 by shmimi            #+#    #+#             */
/*   Updated: 2024/06/03 16:02:35 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server/Server.hpp"
#include "../parsing/Config.hpp"
#include "../parsing/Parser.hpp"

class Config;

struct Request
{
    std::vector<std::string> startLine;
    std::vector< std::pair<std::string, std::string> > headers;
    std::map <std::string, std::string> headers_map;
    std::string body;
    std::string port;
};

class Client: public Config
{
    private:
        Request request;
        int clientFd;

        std::string _method;
        std::string _uri;
        std::string _version;

    public:
        Client(const int clientFd, const std::string& filePath);
        /********** Getters *************/
        int getClientFd() const;
        // int getPort() const;
        const std::vector<std::string> &getRequest() const;
        const std::vector< std::pair<std::string, std::string> >& getHeaders() const;
        const std::map <std::string, std::string>& getHeadersmap() const;
        const std::string& getPort() const;
        const std::string& getBody() const;
        const std::string& getMethod() const;
        const std::string& getUri() const;
        const std::string& getVersion() const;

        /********** Setters *************/
        void setPort();
        void setMethod(const std::string &method);
        void setUri(const std::string &uri);
        void setVersion(const std::string &version);

        void setRequest(const Request &request);

};