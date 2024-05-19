/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 02:36:33 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/16 18:14:18 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server/Server.hpp"
#include "../parsing/Config.hpp"

class Config;

struct Request
{
    std::vector<std::string> startLine;
    std::vector< std::pair<std::string, std::string> > headers;
    std::string body;
};

class Client: public Config
{
    private:
        // int port;
        int clientFd;
        Request request;

        std::string _method;
        std::string _uri;
        std::string _version;

    public:
        Client(const int clientFd);
        /********** Getters *************/
        int getClientFd() const;
        // int getPort() const;
        const std::vector<std::string> &getRequest() const;
        const std::vector< std::pair<std::string, std::string> >& getHeaders() const;
        const std::string& getBody() const;
        const std::string& getMethod() const;
        const std::string& getUri() const;
        const std::string& getVersion() const;

        /********** Setters *************/
        void setMethod(const std::string &method);
        void setUri(const std::string &uri);
        void setVersion(const std::string &version);

        void setRequest(const Request &request);

};