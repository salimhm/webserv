/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 18:08:33 by shmimi            #+#    #+#             */
/*   Updated: 2024/04/27 22:30:18 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port)
{
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->sockfd  == 0){
        perror("In socket");
        exit(1);
    }
    this->addrlen = sizeof(this->addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(this->getSockfd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(this->getSockfd(), (struct sockaddr *)&addr, sizeof(this->getAddr())) < 0)
    {
        perror("Couldn't bind socket to the given address");
        exit(1);
    }
    if(listen(this->getSockfd(), 10) < 0){
        perror("listen problem");
        exit(1);
    }
    
}

int Server::getSockfd()
{
    return this->sockfd;
}

int Server::getPort()
{
    return this->port;
}

void Server::setPort(int newPort)
{
    this->port = newPort;
}

const sockaddr_in &Server::getAddr() const
{
    return this->addr;
}

socklen_t& Server::getAddrlen()
{
    return this->addrlen;
}

std::vector<pollfd> &Server::getClientSockets()
{
    return this->clientSockets;
}

void Server::addClient(int clientFd)
{
    struct pollfd clientSocket;
    clientSocket.fd = clientFd;
    clientSocket.events = POLLIN;
    this->clientSockets.push_back(clientSocket);
}