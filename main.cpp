/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:10:14 by shmimi            #+#    #+#             */
/*   Updated: 2024/03/31 16:46:48 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


int main(int ac, char **av)
{
    (void)ac;
    (void)av;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Couldn't open socket");
        return 1;
    }
    else
    {
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080);
        addr.sin_addr.s_addr = INADDR_ANY;
        if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            perror("Couldn't bind socket to the given address");
            return 1;
        }
        listen(sockfd, 10);
        int client = accept(sockfd, (struct sockaddr *)&addr, &addrlen);
        char buffer[512];
        int data = recv(client, buffer, sizeof(buffer), 0);
        while (data < 0)
        {
            std::cout << buffer << std::endl;
            data = recv(client, buffer, sizeof(buffer), 0);
        }
        buffer[data] = '\0';
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Hello from the server!</h1>";
        send(client, response.c_str(), response.length(), 0);

        /******************************************** Serve multiple clients **************************************/
        // while(1)
        // {
            
        // }
        
        close(client);
        close(sockfd);
    }
}