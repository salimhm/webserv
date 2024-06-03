/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:10:14 by shmimi            #+#    #+#             */
/*   Updated: 2024/06/03 16:58:13 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include "client/Client.hpp"
#include "parsing/Config.hpp"
#include "utils.hpp"
#include "client/clientHandler.hpp"
#include "parsing/Mime.hpp"
#include "parsing/Parser.hpp"

std::vector<Server> setupServer(Config &config, std::vector<pollfd> &pollfds)
{
    std::vector<Server> servers;
    for (size_t i = 0; i < config.getPort().size(); i++)
    {
        std::cout << "Listening on port " << config.getPort()[i] << std::endl;
        Server server(config.getPort()[i]);
        server.setPort(config.getPort()[i]);
        pollfd clienSocket = server.addClient(server.getSockfd(), 1);
        servers.push_back(server);
        pollfds.push_back(clienSocket);
    }
    return servers;
}

std::string getRequest(int clientSocket)
{
    char buffer[1024] = {0};
    int data = recv(clientSocket, buffer, 1024, 0);
    std::string request(buffer);
    if (data < 0)
    {
        perror("recv");
        return "";
    }
    while (data > 0)
    {
        data = recv(clientSocket, buffer, 1024, 0);
        if (data > 0)
            request += buffer;
    }
    // std::cout << "***************REQUEST***************\n";
    // std::cout << request << std::endl;
    // std::cout << "***************END REQUEST***************\n";
    return request;
}

int handleNewConnection(Server &server, std::vector<pollfd> &pollfds, std::vector<Client> &clients, std::string& filePath)
{
    int clientSocket = accept(server.getSockfd(), (struct sockaddr *)&server.getAddr(), &server.getAddrlen());
    if (clientSocket < 0)
    {
        perror("accept");
        exit(1); // Throw
    }
    std::cout << "clientSocket: " << clientSocket << std::endl;
    pollfd socket = server.addClient(clientSocket, 0);
    pollfds.push_back(socket);
    Client client(clientSocket, filePath);
    clients.push_back(client);
    // std::cout << "New connection" << std::endl;
    return clientSocket;
}

int main(int ac, char **av)
{
    (void)av;
    (void)ac;

    {
        try
        {
            std::string filePath = "webserv.yml";
            Config config(filePath);
            config.getAutoIndex();
            
            std::vector<pollfd> pollfds;
            std::vector<Client> clients;
            std::vector<Server> servers = setupServer(config, pollfds);

            int clientSocket;
            std::string request;

            std::string response;
            while (1)
            {
                try
                {
                    for (size_t j = 0; j < pollfds.size(); j++) // Loop through all the servers (ports)
                    {
                        int ready = poll(pollfds.data(), pollfds.size(), -1);
                        if (ready < 0)
                        {
                            perror("poll");
                            exit(1);
                        }
                        for (size_t i = 0; i < servers.size(); i++)
                        {
                            if (pollfds[j].fd == servers[i].getSockfd() && pollfds[j].revents & POLLIN)
                            {
                                clientSocket = handleNewConnection(servers[i], pollfds, clients, filePath);
                            }
                        }
                        for (size_t k = 0; k < clients.size(); k++)
                        {
                            if (pollfds[j].fd == clients[k].getClientFd())
                            {
                                if (pollfds[j].revents & POLLIN)
                                {
                                    request = getRequest(clients[k].getClientFd());
                                    if (request.size() > 0)
                                    {
                                        clients[k].setRequest(parseRequest(request));
                                        response = handleRequest(clients[k], config, request);
                                    }
                                }
                            }
                            if (pollfds[j].fd == clients[k].getClientFd() && pollfds[j].revents & POLLOUT)
                            {
                                size_t bytes = send(clients[k].getClientFd(), response.c_str(), response.size(), 0);
                                if (bytes < 0)
                                {
                                    std::cerr << "Error sending data" << std::endl;
                                    clients.erase(clients.begin() + k);
                                    pollfds.erase(pollfds.begin() + j);
                                }
                                std::cout << "Bytes sent: " << bytes << std::endl;
                                close(clients[k].getClientFd());
                                clients.erase(clients.begin() + k);
                                pollfds.erase(pollfds.begin() + j);
                            }
                        }
                    }
                }
                catch (const std::exception &e)
                {
                    std::cerr << e.what() << std::endl;
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}