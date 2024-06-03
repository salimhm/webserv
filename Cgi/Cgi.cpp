#include "Cgi.hpp"
#include <fcntl.h>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <signal.h>
#include "../server/Default.hpp"

Cgi::Cgi(int filefd, std::string& request)
{
    this->filefd = filefd;
    this->request = request;
}

void Cgi::getCgi(Client& client)
{
    std::map<std::string, std::string> headers = client.getHeadersmap();
    std::string boundry = "CONTENT_TYPE=" + headers["Content-Type"].erase(0, 1);
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
            // std::cout << 
        }
    }
    close(tmpoutfile);
}

void Cgi::postCgi(Client &client, Config &config)
{
    std::map<std::string, std::string> headers = client.getHeadersmap();
    std::string boundry = "CONTENT_TYPE=" + headers["Content-Type"].erase(0, 1);
    std::string method = "REQUEST_METHOD=POST";
    std::string content_len = "CONTENT_LENGTH=" + headers["Content-Length"].erase(0, 1);
    std::string parspath = request;
    std::string path =  "." + parspath.substr(parspath.find("POST") + 5, parspath.find(" H") - (parspath.find("POST") + 5));

    remove("/tmp/in.txt");
    remove("responsepostCGI.html");
    int tmpinfile = open("/tmp/in.txt", O_CREAT | O_RDWR, 0777);
    int tmpoutfile = open("responsepostCGI.html", O_CREAT | O_RDWR, 0777);

    if (std::atoi(config.getClientMaxBodySize().c_str()) >= std::atoi(headers["Content-Length"].erase(0, 1).c_str()))
    {
        char *env[4];
        env[0] = (char *)method.c_str();
        env[1] = (char *)boundry.c_str();
        env[2] = (char *)content_len.c_str();
        env[3] = NULL;


        clock_t start;
        clock_t end;
        int status;

        write(tmpinfile, client.getBody().c_str(), client.getBody().size());
        // std::cout << "CGI == " << client.getBody().c_str() << "\n\n" << this->request.size() << std::endl;
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
                if ((end - start) / CLOCKS_PER_SEC > 5)
                {
                    kill(pid, SIGKILL);
                    
                    break;
                }
            }
            if (WEXITSTATUS(status) > 0)
            {
                std::cerr << "error: the process exit" << WEXITSTATUS(status) << std::endl;
            }
            close(tmpinfile);
            close(tmpoutfile);
        }
    }
    else
    {
        std::map<std::string, std::string> isErrorPage = config.getIsErrorPage();
        if (isErrorPage.find("413") != isErrorPage.end())
        {
            std::string error413 = readFile(isErrorPage["413"]);
            write(tmpoutfile, error413.c_str(), error413.size());
        }
        else
        {
            Default def;
            std::string error413 = def.generateErrorPage("413");
            write(tmpoutfile, error413.c_str(), error413.size());
        }
    }
    close(tmpinfile);
    close(tmpoutfile);
}