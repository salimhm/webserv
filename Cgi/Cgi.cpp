/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:47:13 by abouram           #+#    #+#             */
/*   Updated: 2024/06/09 18:36:35 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include <fcntl.h>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <signal.h>
#include "../server/Default.hpp"
#include <cstdlib>

Cgi::Cgi(std::string& request)
{
    this->request = request;
}

void Cgi::getCgi(Client& client)
{
    std::map<std::string, std::string> headers = client.getHeadersmap();
    // std::map<std::string, std::string>::iterator it = headers.begin();
    // for(;it != headers.end(); it++)
    // {
    //     for (size_t i = 0;i < it->first.size(); i++)
    //     std::tolower(it->first[i]);
    // }
    std::string boundry = "CONTENT_TYPE=" + headers["content-type"].erase(0, 1);
    std::string querystr = "QUERY_STRING=" + request.substr(request.find("?") + 1 , request.find(" H") - (request.find("?") + 1));
    std::string method = "REQUEST_METHOD=GET";
    std::string parspath = request;
    std::string path = "." + parspath.substr(parspath.find("GET") + 4, parspath.find("?") - (parspath.find("GET") + 4));
    int status;
    char *env[4];
    env[0] = (char *)querystr.c_str();
    env[1] = (char *)method.c_str();
    env[2] = (char *)boundry.c_str();
    env[3] = NULL;

    remove("responsegetCGI.html");
    int tmpoutfile = open("responsegetCGI.html", O_CREAT | O_RDWR, 0777);
    pid_t pid = fork();
    if (pid == 0)
    {
        dup2(tmpoutfile, 1);
        execve(path.c_str(), NULL, env);
    }
    else if (pid > 0)
    {
        while(!waitpid(pid, &status, WNOHANG))
        {
            if (WEXITSTATUS(status) > 0)
                std::cerr << "error: the process exit" << WEXITSTATUS(status) << std::endl;
        }
    }
    close(tmpoutfile);
}

const std::string Cgi::postCgi(Client &client, Config &config)
{
    std::map<std::string, std::string> headers = client.getHeadersmap();
    std::string boundry = "CONTENT_TYPE=" + headers["content-type"].erase(0, 1);
    std::string method = "REQUEST_METHOD=POST";
    std::stringstream ss;
    ss << client.getAllRequest().size();
    std::string content_len = "CONTENT_LENGTH=" + ss.str();
    std::string parspath = client.getAllRequest();
    std::string path =  "." + parspath.substr(parspath.find("POST") + 5, parspath.find(" H") - (parspath.find("POST") + 5));
    std::string body = client.getBody();

    remove("/tmp/in.txt");
    remove("responsepostCGI.html");
    int tmpoutfile = open("responsepostCGI.html", O_CREAT | O_RDWR, 0777);
    int tmpinfile = open("/tmp/in.txt", O_CREAT | O_RDWR, 0777);

    std::cout << "CGI>>>>>>>>>>>" << content_len << std::endl;

    if (std::atoll(config.getClientMaxBodySize().c_str()) >= std::atoll(headers["content-length"].c_str()))
    {
        std::cout << std::atoll(config.getClientMaxBodySize().c_str()) << "  " << std::atoll(headers["content-length"].c_str()) << std::endl;
        char *env[4];
        env[0] = (char *)method.c_str();
        env[1] = (char *)boundry.c_str();
        env[2] = (char *)content_len.c_str();
        env[3] = NULL;

        clock_t start;
        clock_t end;
        int status;
        if (headers["transfer-encoding"] == " chunked")
        {
            std::string hexline = body.substr(0, body.find("\r\n"));
            size_t dec = strtol(hexline.c_str(), NULL, 16);
            std::string str = body.substr(hexline.size() + 2, dec);
            std::string temp;
            body.erase(0, dec + hexline.size() + 2);
            while (hexline != "0")
            {
                hexline = body.substr(0, body.find("\r\n"));
                dec = strtol(hexline.c_str(), NULL, 16);
                temp = body.substr(hexline.size() + 2, dec);
                str.append(temp);
                body.erase(0, dec + hexline.size() + 2);
            }
            client.setBody(str);
            return "chunked";
        }
        else
        {
            write(tmpinfile, client.getAllRequest().c_str(), client.getAllRequest().size());
            close(tmpinfile);
            tmpinfile = open("/tmp/in.txt", O_RDONLY, 0777);
            start = clock();
            pid_t pid = fork();
            if (pid == 0)
            {
                dup2(tmpinfile, 0);
                dup2(tmpoutfile, 1);
                execve(path.c_str(), NULL, env);
            }
            else if (pid > 0)
            {
                while (!waitpid(pid, &status, WNOHANG))
                {
                    end = clock();
                    if ((end - start) / CLOCKS_PER_SEC > 1)
                    {
                        kill(pid, SIGKILL);
                        return "504";
                        break;
                    }
                }
                if (WEXITSTATUS(status) > 0)
                {
                    if(WEXITSTATUS(status) > 0)
                    {
                        return "502";   
                    }
                }
                close(tmpinfile);
                close(tmpoutfile);
            }
        }
    }
    else
    {
        return "413";
    }
    close(tmpinfile);
    close(tmpoutfile);
    return "";
}