/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:10:14 by shmimi            #+#    #+#             */
/*   Updated: 2024/04/30 08:05:00 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Config.hpp"
#include "Client.hpp"
#include "utils.hpp"

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

struct Request parseRequest(const std::string &request)
{
    struct Request httpRequest;

    std::string method;
    std::string path;
    std::string version;
    std::string body;

    std::string headerString;

    int pos1 = request.find(' ') + 1;
    int pos2 = request.find(' ', pos1) + 1;
    int pos3 = request.find('\n', pos2);
    int crlf = request.find("\r\n\r\n");

    method = request.substr(0, pos1 - 1);
    path = request.substr(pos1, pos2 - pos1 - 1);
    version = request.substr(pos2, pos3 - pos2 - 1);
    body = request.substr(crlf + 4);

    headerString = request.substr(request.find("\r\n"), crlf - body.size() - 2);
    std::cout << "************ Printing headers *************";
    std::cout << headerString << std::endl;
    std::cout << "************ END Printing headers *************\n";

    std::vector<std::string> splittedHeader;
    std::vector<std::string> splittedHeader2;
    splittedHeader = split(headerString, "\r\n");

    for (size_t i = 0; i < splittedHeader.size(); i++)
    {
        splittedHeader2 = split(splittedHeader[i], ": ");
        if (splittedHeader2[0].size() > 0)
            httpRequest.headers[splittedHeader2[0]] = splittedHeader2[1];
    }

    httpRequest.startLine.push_back(method);
    httpRequest.startLine.push_back(path);
    httpRequest.startLine.push_back(version);
    if (body.size() > 0)
        httpRequest.body = body;
    // std::cout << "************ Start Line *************\n";
    // for (size_t i = 0; i < httpRequest.startLine.size(); i++)
    // {
    //     std::cout << httpRequest.startLine[i] << " " << httpRequest.startLine[i].size() << std::endl;
    // }
    // std::cout << "************ END Start Line *************\n";
    std::cout << "************ Printing headers *************\n";
    std::map<std::string, std::string>::iterator it = httpRequest.headers.begin();
    while (it != httpRequest.headers.end())
    {
        std::cout << it->first << "=>" << it->second << std::endl;
        it++;
    }
    std::cout << "************ END headers *************\n";
    // std::cout << "************ Body *************\n";
    // std::cout << httpRequest.body << std::endl;
    // std::cout << "************ END Body *************\n";
    return httpRequest;
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
                            std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 23\n\n<h1>Hello world!!!</h1>";
                            send(clientSocket, response.c_str(), response.size(), 0);
                            close(clientSocket);
                        }
                    }
                }
            }
        }
    }
}