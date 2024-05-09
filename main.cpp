/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:10:14 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/09 23:10:38 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include "client/Client.hpp"
#include "Config.hpp"
#include "utils.hpp"
#include "client/clientHandler.hpp"
#include "server/Mime.hpp"

std::vector<Server> setupServer(Config config)
{
    std::vector<Server> servers;
    // std::cout << "Num of ports " << config.getPort().size() << std::endl;
    for (size_t i = 0; i < config.getPort().size(); i++)
    {
        std::cout << "Listening on port " << config.getPort()[i] << std::endl;
        Server server(config.getPort()[i]);
        server.setPort(config.getPort()[i]);
        server.addClient(server.getSockfd());
        servers.push_back(server);
    }
    return servers;
}

void handleNewConnection(Server& server)
{
    int clientSocket = accept(server.getSockfd(), (struct sockaddr *)&server.getAddr(), &server.getAddrlen());
    if (clientSocket < 0)
    {
        perror("accept");
        exit(1); // Throw
    }
    server.addClient(clientSocket);
    std::cout << "New connection" << std::endl;

    char buffer[1024] = {0};
    int data = recv(clientSocket, buffer, 1024, 0);
    if (data < 0)
    {
        perror("recv");
        return;
    }
    std::string request(buffer);
    while (data > 0)
    {
        // std::cout << buffer << std::endl;
        data = recv(clientSocket, buffer, 1024, 0);
        request += buffer;
    }
    std::cout << "***************REQUEST***************\n";
    std::cout << request << std::endl;
    std::cout << "***************END REQUEST***************\n";
}

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
        std::vector<Server> servers = setupServer(config);

        std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";
        while (1)
        {
            try
            {
                // std::cout << "Here\n";
                for (size_t j = 0; j < servers.size(); j++) // Loop through all the servers (ports)
                {
                    int ready = poll(servers[j].getClientSockets().data(), servers[j].getClientSockets().size(), 50);
                    if (ready < 0)
                    {
                        perror("poll");
                        exit(1);
                    }
                    if (servers[j].getClientSockets()[0].revents & POLLIN)
                    {
                        handleNewConnection(servers[j]);
                    }
                    // std::cout << "Client sockets size: " << servers[j].getClientSockets().size() << "\n";
                    for (size_t i = 1; i < servers[j].getClientSockets().size(); i++) //Loop through all clients
                    {
                        if (servers[j].getClientSockets()[i].revents & POLLOUT)
                        {
                            send(servers[j].getClientSockets()[i].fd, response.c_str(), response.size(), 0);
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