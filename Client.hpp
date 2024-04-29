/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 02:36:33 by shmimi            #+#    #+#             */
/*   Updated: 2024/04/29 17:55:04 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

struct Request
{
    std::vector<std::string> startLine;
    std::map<std::string, std::string> headers;
};

class Client
{
    private:
        int port;
        int clientFd;
        Request request;

    public:
        Client(const int port, const int clientFd, const Request &request);
        /********** Getters *************/
        int getClientFd() const;
        int getPort() const;
        const std::vector<std::string> &getRequest() const;
        const std::map<std::string, std::string>& getHeaders() const;
};