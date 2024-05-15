/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:10:14 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/15 19:09:26 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include "client/Client.hpp"
#include "Config.hpp"
#include "utils.hpp"
#include "client/clientHandler.hpp"
#include "server/Mime.hpp"

std::vector<Server> setupServer(Config config, std::vector<pollfd> &pollfds)
{
    std::vector<Server> servers;
    // std::cout << "Num of ports " << config.getPort().size() << std::endl;
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

std::string getRequest(int clientSocket, std::vector<Client> &clients)
{
    // std::cout << "From recv => " << clientSocket << std::endl;
    // (void)server;
    (void)clients;
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
        // std::cout << buffer << std::endl;
        data = recv(clientSocket, buffer, 1024, 0);
        request += buffer;
    }
    // if (request.size() > 0)
    // {
    //     Client client(server.getPort(), clientSocket, parseRequest(request));
    //     clients.push_back(client);
    // }
    // std::cout << "***************REQUEST***************\n";
    // std::cout << request << std::endl;
    // std::cout << "***************END REQUEST***************\n";
    return request;
}

int handleNewConnection(Server &server, std::vector<pollfd> &pollfds, std::vector<Client> &clients)
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
    Client client(clientSocket);
    clients.push_back(client);
    // std::cout << "New connection" << std::endl;
    return clientSocket;
}

// void handlePollout(std::string &response, std::vector<Client> &clients, int k, Config &config, pollfd &pollfd)
// {
//     std::cout << "Here POLLOUT\n";
//     response = handleRequest(clients[k], config);
//     send(clients[k].getClientFd(), response.c_str(), response.size(), 0);
//     close(clients[k].getClientFd());
//     clients.erase(clients.begin() + k);
//     pollfd.revents = 0;
//     pollfd.events = POLLIN;
// }

int main(int ac, char **av)
{
    (void)av;
    (void)ac;

    std::ifstream file("webserv.conf");
    Config config;

    config.getClientMaxBodySize();

    if (!file.is_open())
    {
        std::cerr << "Couldn't open config file!\n";
        return 1;
    }
    else
    {
        std::vector<pollfd> pollfds;
        std::vector<Server> servers = setupServer(config, pollfds);
        std::vector<Client> clients;

        int clientSocket;
        std::string request;

        for (size_t i = 0; i < servers.size(); i++)
        {
            std::cout << "Server sockets are: " << servers[i].getSockfd() << std::endl;
        }

        std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";
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
                            clientSocket = handleNewConnection(servers[i], pollfds, clients);
                        }
                    }
                    for (size_t k = 0; k < clients.size(); k++)
                    {
                        if (pollfds[j].fd == clients[k].getClientFd())
                        {
                            if (pollfds[j].revents & POLLIN)
                            {
                                request = getRequest(clients[k].getClientFd(), clients);
                                if (request.size() > 0)
                                {
                                    clients[k].setRequest(parseRequest(request));
                                    response = handleRequest(clients[k], config);
                                }
                            }
                        }
                        if (pollfds[j].fd == clients[k].getClientFd() && pollfds[j].revents & POLLOUT)
                        {
                            int bytes = send(clients[k].getClientFd(), response.c_str(), response.size(), 0);
                            std::cout << "Bytes sent: " << bytes << std::endl;
                            close(clients[k].getClientFd());
                            clients.erase(clients.begin() + k);
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
}