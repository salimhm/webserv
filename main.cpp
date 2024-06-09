/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:10:14 by shmimi            #+#    #+#             */
/*   Updated: 2024/06/09 19:00:00 by shmimi           ###   ########.fr       */
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
        Server server(config.getPort()[i], config.getHost(config.getPort()[i]));
        server.setPort(config.getPort()[i]);
        pollfd clienSocket = server.addClient(server.getSockfd(), 1);
        servers.push_back(server);
        pollfds.push_back(clienSocket);
    }
    for (size_t i = 0; i < config.getPort().size(); i++)
        std::cout << "Listening on " << config.getHost(config.getPort()[i]) << ":" << config.getPort()[i] << std::endl;
    return servers;
}

std::string getRequest(Client &client)
{
    char buffer[10024] = {0};
    int data = recv(client.getClientFd(), buffer, sizeof(buffer), 0);
    if (data < 0)
        return "";
    if (data == 0)
        return "";

    std::string request(buffer, data);

    client.setBytesRead(request);

    if (!client.getIsHeaderParser())
    {
        if (getHeaders(client.getBytesRead(), client))
            client.setisHeaderParser();
        std::map<std::string, std::string> headers = client.getHeadersmap();
        size_t portIndex = headers["host"].erase(0, 1).find(":");
        std::string port = headers["host"].erase(0, 1).substr(portIndex);
        client.setPort(port);
    }

    if (client.getIsHeaderParser())
    {
        std::map<std::string, std::string> headers = client.getHeadersmap();
        if (headers.find("content-length") != headers.end())
        {
            
            size_t contentLength = std::stoul(headers["content-length"]);
            size_t remainingBody = contentLength - client.getBody().size();
            if (client.getBytesRead().size() >= remainingBody)
            {
                client.setBody(client.getBytesRead().substr(0, remainingBody));
                client.getBytesRead().erase(0, remainingBody);
            }
            else
            {
                client.setBody(client.getBytesRead());
                client.getBytesRead().clear();
            }
        }
        else
        {
            client.setBody(client.getBytesRead());
            client.getBytesRead().clear();
        }
    }
    client.setAllRequest(request);
    return client.getBody();
}

int handleNewConnection(Server &server, std::vector<pollfd> &pollfds, std::vector<Client> &clients, std::string &filePath)
{
    int clientSocket = accept(server.getSockfd(), (struct sockaddr *)&server.getAddr(), &server.getAddrlen());
    if (clientSocket < 0)
        throw std::runtime_error("Error accepting connection");
    pollfd socket = server.addClient(clientSocket, 0);
    pollfds.push_back(socket);
    Client client(clientSocket, filePath);
    clients.push_back(client);
    return clientSocket;
}

int main(int ac, char **av)
{
    (void)av;
    (void)ac;

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
        ssize_t bytes;
        std::map<std::string, std::string> headers;
        while (1)
        {
            try
            {
                for (size_t j = 0; j < pollfds.size(); j++) // Loop through all the servers (ports)
                {
                    int ready = poll(pollfds.data(), pollfds.size(), -1);
                    if (ready < 0)
                    {
                        throw std::runtime_error("Error polling");
                    }
                    for (size_t i = 0; i < servers.size(); i++)
                    {
                        if (pollfds[j].fd == servers[i].getSockfd() && pollfds[j].revents & POLLIN)
                            clientSocket = handleNewConnection(servers[i], pollfds, clients, filePath);
                    }
                    for (size_t k = 0; k < clients.size(); k++)
                    {
                        if (pollfds[j].fd == clients[k].getClientFd() && pollfds[j].revents & POLLIN)
                        {
                            request = getRequest(clients[k]);
                            if (request.empty())
                            {
                                close(clients[k].getClientFd());
                                clients.erase(clients.begin() + k);
                                pollfds.erase(pollfds.begin() + j);
                                break;
                            }
                            std::map<std::string, std::string> headers = clients[k].getHeadersmap();
                            if (clients[k].getIsHeaderParser())
                            {
                                if (clients[k].getMethod() == "POST")
                                {
                                    size_t contentLength = std::stoul(headers["content-length"]);
                                    if (clients[k].getBody().size() >= contentLength)
                                        response = handleRequest(clients[k], config, request);
                                    else
                                        continue;
                                }
                                else if (clients[k].getMethod() == "GET")
                                    response = handleRequest(clients[k], config, request);
                                else if (clients[k].getMethod() == "DELETE")
                                    response = handleRequest(clients[k], config, request);
                            }
                        }
                        if (pollfds[j].fd == clients[k].getClientFd() && pollfds[j].revents & POLLOUT)
                        {
                            if (clients[k].getTotalBytes() == 0)
                            {
                                headers = clients[k].getHeadersmap();
                                clients[k].setTotalBytes(response.size());
                                clients[k].setBytesToSend(response.size() - clients[k].getBytesSent());
                            }
                            bytes = send(clients[k].getClientFd(), response.c_str() + clients[k].getBytesSent(), clients[k].getBytesToSend(), 0);
                            clients[k].setBytesSent(clients[k].getBytesSent() + bytes);
                            clients[k].setBytesToSend(clients[k].getBytesToSend() - bytes);
                            if (bytes < 0)
                            {
                                close(clients[k].getClientFd());
                                clients.erase(clients.begin() + k);
                                pollfds.erase(pollfds.begin() + j);
                            }
                            if (bytes == 0)
                            {
                                close(clients[k].getClientFd());
                                clients.erase(clients.begin() + k);
                                pollfds.erase(pollfds.begin() + j);
                            }
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