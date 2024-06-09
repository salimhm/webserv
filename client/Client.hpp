/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 02:36:33 by shmimi            #+#    #+#             */
/*   Updated: 2024/06/09 02:27:41 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server/Server.hpp"
#include "../parsing/Config.hpp"
#include "../parsing/Parser.hpp"

class Config;

// struct Request
// {
//     std::vector<std::string> startLine;
//     std::vector< std::pair<std::string, std::string> > headers;
//     std::map <std::string, std::string> headers_map;
//     std::string body;
//     std::string port;
// };

class Client: public Config
{
    private:
        // Request request;
        int clientFd;

        std::string _method;
        std::string _uri;
        std::string _version;


        int _totalBytes;
        int _bytesToSend;
        int _bytesSent;

        std::vector<std::string> startLine;
        std::vector< std::pair<std::string, std::string> > headers;


    public:

        std::map <std::string, std::string> headers_map;
        std::string body;
        std::string port;

        int crlf;
        std::string bytesRead;
        int headersParsed;
        
        std::string allRequest;

        Client(const int clientFd, const std::string& filePath);
        /********** Getters *************/
        int getClientFd() const;
        const std::vector<std::string> &getRequest() const;
        const std::vector< std::pair<std::string, std::string> >& getHeaders() const;
        const std::map <std::string, std::string>& getHeadersmap() const;
        const std::string& getPort() const;
        const std::string& getBody() const;
        const std::string& getMethod() const;
        const std::string& getUri() const;
        const std::string& getVersion() const;

        const int &getTotalBytes();
        const int &getBytesToSend();
        const int &getBytesSent();
        /********** Setters *************/
        void setPort(const std::string& port);
        void setMethod(const std::string &method);
        void setUri(const std::string &uri);
        void setVersion(const std::string &version);
        void setBody(const std::string& body);

        void setTotalBytes(const int &totalBytes);
        void setRemainingBytes(const int &remainingBytes);
        void setBytesToSend(const int& bytesToSend);
        void setBytesSent(const int& bytesSent);

        void addStartLine(const std::string& line);
        void setHeaders(const std::vector< std::pair<std::string, std::string> >& headers);
};