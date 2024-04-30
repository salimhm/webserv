/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 14:58:46 by shmimi            #+#    #+#             */
/*   Updated: 2024/04/29 13:15:16 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <vector>
#include <fstream>
#include <map>

class Server
{
private:
    int sockfd;
    struct sockaddr_in addr;
    socklen_t addrlen;
    std::vector<pollfd> clientSockets;
    int port;

public:
    Server(int port);
    /************ Getters ***************/
    int getPort();
    int getSockfd();
    std::vector<pollfd> &getClientSockets();
    const sockaddr_in &getAddr() const;
    socklen_t &getAddrlen();
    
    /* Setters*/
    void setPort(int port);

    void addClient(int clientFd);
};