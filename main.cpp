/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:10:14 by shmimi            #+#    #+#             */
/*   Updated: 2024/04/28 07:36:57 by shmimi           ###   ########.fr       */
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

std::vector<std::string> parseRequest(std::string request)
{
    std::vector<std::string> newRequest;
    
    std::string method;
    std::string path;
    std::string version;
    
    int pos1 = request.find(' ') + 1;
    int pos2 = request.find(' ', pos1) + 1;
    int pos3 = request.find('\n', pos2);
    
    method = request.substr(0, pos1 - 1);
    path = request.substr(pos1, pos2 - pos1 - 1);
    version = request.substr(pos2, pos3 - pos2 - 1);

    newRequest.push_back(method);
    newRequest.push_back(path);
    newRequest.push_back(version);
    // for (size_t i = 0; i < newRequest.size(); i++)
    // {
    //     std::cout << newRequest[i] << " " << newRequest[i].size() << std::endl;
    // }
    return newRequest;
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
        
        std::vector<Client> clients;

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
                            std::cout << "New connection " << conn << std::endl << std::endl << std::endl;
                            char buffer[10] = {0};
                            int data = recv(new_socket, buffer, sizeof(buffer), 0);
                            buffer[data] = '\0';
                            std::string request(buffer); // Convert char* to string
                            while (data > 0)
                            {
                                if (data != sizeof(buffer))
                                    break;
                                data = recv(new_socket, buffer, sizeof(buffer), 0);
                                buffer[data] = '\0';
                                request += buffer;
                            }
                            Client client(servers[j].getPort(), new_socket, parseRequest(request));
                            clients.push_back(client);
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