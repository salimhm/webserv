/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:10:14 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/02 00:01:07 by shmimi           ###   ########.fr       */
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

int main(int ac, char **av)
{
    (void)av;
    (void)ac;

    std::ifstream file("webserv.conf");
    Config config;

    Mime mime;
    // mime.getExtension();

    // std::map<std::vector<std::string>, std::string> mimeMap;

    // // Sample data
    // std::string line = ".ma, .mb, .nb|application/mathematica";
    // std::string extensions = line.substr(0, line.find('|'));
    // std::string value = line.substr(line.find('|') + 1);

    // // Split extensions by ", " using your split function
    // std::vector<std::string> keys = split(extensions, ", ");

    // // Insert into mimeMap
    // mimeMap[keys] = value;

    // Access elements in mimeMap
    // std::map<std::vector<std::string>, std::string>::iterator it = mimeMap.begin();
    // while (it != mimeMap.end()) {
    //     std::cout << it->first[2] << " ==>" << it->second << std::endl;
    //     it++;
    // }

    // std::cout << split(".ma, .mb, .nb|application/mathematica", "|")[1] << std::endl;   


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
            try
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
                                int clientSocket = accept(servers[j].getSockfd(), (struct sockaddr *)&servers[j].getAddr(), (socklen_t *)&servers[j].getAddrlen());
                                if (clientSocket < 0)
                                {
                                    perror("accept");
                                    exit(1);
                                }
                                conn++;
                                std::cout << "New connection " << conn << std::endl
                                          << std::endl
                                          << std::endl;
                                char buffer[512] = {0};
                                int data = recv(clientSocket, buffer, sizeof(buffer), 0);
                                buffer[data] = '\0';
                                std::string request(buffer); // Convert char* to string
                                while (data > 0)
                                {
                                    if (data != sizeof(buffer))
                                        break;
                                    data = recv(clientSocket, buffer, sizeof(buffer), 0);
                                    buffer[data] = '\0';
                                    request += buffer;
                                }
                                std::cout << "****** REQUEST ********" << std::endl;
                                std::cout << request << std::endl;
                                std::cout << "****** END REQUEST ********" << std::endl;
                                Client client(servers[j].getPort(), clientSocket, parseRequest(request));
                                clients.push_back(client);
                                servers[j].addClient(clientSocket);
                                // std::string response = "HTTP/1.1 200 OK\nContent-Type: text/text\nContent-Length: 21\n\nHelloThisisfrom lolol";
                                std::string response = handleRequest(clients[0], mime);
                                send(clientSocket, response.c_str(), response.size(), 0);
                                close(clientSocket);
                                // servers[j].getClientSockets().pop_back(); // Remove the client socket from the pollfd vector
                                // clients.pop_back();
                            }
                        }
                    }
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << std::endl;
            }
            if (clients.size() > 0)
            {
                // std::cout << "Num of clients " << clients.size() << std::endl;
                // handleRequest(clients[0]);
            }
        }
    }
}