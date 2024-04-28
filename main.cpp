/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:10:14 by shmimi            #+#    #+#             */
/*   Updated: 2024/04/28 05:53:05 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Config.hpp"
#include "Client.hpp"

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

int main(int ac, char **av)
{
    (void)av;
    (void)ac;
    std::ifstream file("webserv.conf");

    Config config;
    // std::cout << "Port " << config.getPort()[0] << std::endl;
    // std::cout << "Server_name " << config.getServerName() << std::endl;
    // std::cout << "Root " << config.getRoot() << std::endl;

    if (!file.is_open())
    {
        std::cerr << "Couldn't open config file!\n";
        return 1;
    }
    else
    {
        std::vector<Server> servers = setupServer(config);

        int conn = 0;
        while (1)
        {
            for (size_t j = 0; j < servers.size(); j++) // Loop through all the servers (ports)
            {
                int ready = poll(servers[j].getClientSockets().data(), servers[j].getClientSockets().size(), 50);
                if (ready < 0)
                {
                    perror("poll");
                    exit(1);
                }
                for (size_t i = 0; i < servers[j].getClientSockets().size(); i++) // Loop through all the client sockets
                {
                    if (servers[j].getClientSockets()[i].fd == servers[j].getSockfd()) // If it's the server socket
                    {
                        if (servers[j].getClientSockets()[i].revents & POLLIN) // If there is a new connection
                        {
                            int new_socket = accept(servers[j].getSockfd(), (struct sockaddr *)&servers[j].getAddr(), (socklen_t *)&servers[j].getAddrlen());
                            if (new_socket < 0)
                            {
                                perror("accept");
                                exit(1);
                            }
                            conn++;
                            std::cout << "New connection " << conn << std::endl;
                            char buffer[10] = {0};
                            int data = recv(new_socket, buffer, sizeof(buffer), 0);
                            buffer[data] = '\0';
                            std::string str(buffer); // Convert char* to string
                            while (data > 0)
                            {
                                if (data != sizeof(buffer))
                                    break;
                                data = recv(new_socket, buffer, sizeof(buffer), 0);
                                buffer[data] = '\0';
                                str += buffer;
                            }
                            std::cout << str << std::endl;
                            servers[j].addClient(new_socket);
                            std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 23\n\n<h1>Hello world!!!</h1>";
                            // if (new_socket == servers[j].getSockfd()) // Protection against if the server socket is the client socket (can't send to itself)
                            //     continue;
                            send(new_socket, response.c_str(), response.size(), 0);
                            close(new_socket);
                        }
                    }
                }
            }
        }
    }
}