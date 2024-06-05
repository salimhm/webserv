/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 02:07:06 by shmimi            #+#    #+#             */
/*   Updated: 2024/06/05 15:00:02 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clientHandler.hpp"
#include "../server/Response.hpp"
#include "../server/Default.hpp"
#include <sstream>

struct Request parseRequest(const std::string &request)
{
    try
    {
        struct Request httpRequest;

        std::string method;
        std::string uri;
        std::string version;
        std::string body;
        std::vector<std::pair<std::string, std::string> > headers;

        std::string headerString;

        int pos1 = request.find(' ') + 1;
        int pos2 = request.find(' ', pos1) + 1;
        int pos3 = request.find('\n', pos2);
        int crlf = request.find("\r\n\r\n");

        method = request.substr(0, pos1 - 1);
        uri = request.substr(pos1, pos2 - pos1 - 1);

        uri = normalizeUrl(uri);

        // std::cout << "newUri => " << uri << std::endl;

        version = request.substr(pos2, pos3 - pos2 - 1);
        body = request.substr(crlf + 4);

        headerString = request.substr(request.find("\r\n"), crlf - body.size() - 2);

        size_t posHeader = 2;
        int pos = headerString.find("\n", posHeader);
        int start = 0;
        int semiColon = 0;
        while (posHeader != std::string::npos)
        {
            posHeader++;
            pos = headerString.find("\n", posHeader);
            posHeader = headerString.find("\n", pos);
            std::string header = headerString.substr(start, posHeader - start - 1);
            start = posHeader + 1;
            semiColon = header.find_first_of(":");
            std::string key = header.substr(0, semiColon);
            std::string value = header.substr(semiColon + 1);
            if (semiColon < 0)
                break;
            headers.push_back(std::make_pair(key, value));
        }
        for (size_t i = 0; i < headers.size(); i++)
        {
            // std::cout << headers[i].first << "=>" << headers[i].second << std::endl;
            httpRequest.headers_map[headers[i].first] = headers[i].second;
        }

        for (size_t i = 0; i < headers.size(); i++)
        {
            if (headers[i].first == "\r\nHost")
            {
                size_t pos = headers[i].second.find(":");
                if (pos != std::string::npos)
                {
                    httpRequest.port = headers[i].second.substr(pos + 1);
                    break;
                }
                else
                {
                    httpRequest.port = "80";
                    break;
                }
            }
        }
        // std::cout << "Port => " << httpRequest.port << "  " << httpRequest.port.size() << std::endl;

        // std::cout << "************ Printing headers *************";
        // for (size_t i = 0; i < headers.size(); i++)
        // {
        //     std::cout << headers[i].first << "=>" << headers[i].second << std::endl;
        // }
        // std::cout << "************ END Printing headers *************\n";

        httpRequest.startLine.push_back(method);
        httpRequest.startLine.push_back(uri);
        httpRequest.startLine.push_back(version);

        httpRequest.headers = headers;

        if (body.size() > 0)
            httpRequest.body = body;

        return httpRequest;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return Request();
}

std::string getFileExtension(const std::string &filePath)
{
    size_t pos = filePath.find_last_of(".");
    if (pos != std::string::npos)
    {
        std::string extension = filePath.substr(pos);
        return extension;
    }
    return "";
}

std::string readFile(std::string filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Error opening file" << std::endl;
        return "";
    }
    std::string line;
    std::string content;
    while (getline(file, line))
    {
        content += line + "\n";
    }
    // std::cout << "Content Size " << content.size() << std::endl;
    file.close();
    return content;
}

std::string getResponse(Response responseObj)
{
    std::string response;
    // std::string response = "HTTP/1.1 200 OK\nContent-Type: text/text\nContent-Length: 21\n\nHelloThisisfrom lolol";

    response += responseObj.getHttpVersion() + " " + responseObj.getStatus() + " " + responseObj.getStatusMessage() + "\r\n" + responseObj.getContentType() + "\r\n" + responseObj.getContentLength() + "\r\n" + responseObj.getHeaders() + "\r\n\r\n" + responseObj.getBody();
    return response;
}

std::string generateIndex(int statusCode)
{
    switch (statusCode)
    {
    case 403:
        return readFile("./assets/error/403.html");
    case 404:
        return readFile("./assets/error/404.html");
    default:
        return "";
    }
}

void generateResponse(Config& config, Response &response, const std::string &filePath, const std::string &contentType, const std::string &status, const std::string &statusMessage, int autoIndexFlag)
{
    (void)config;
    response.setHttpVersion("HTTP/1.1");
    response.setStatus(status);
    response.setStatusMessage(statusMessage);
    response.setContentType("Content-Type: " + contentType);
    if (autoIndexFlag)
        response.setBody(filePath);
    else
        response.setBody(readFile(filePath));
    std::stringstream bodyLength;
    bodyLength << response.getBody().size();
    response.setContentLength("Content-Length: " + bodyLength.str());
    // if (config.getRedirect().size())
    // {
    //     response.setHeaders("Location: " + config.getRedirect()[1]);
    //     response.setStatus("301");
    //     response.setStatusMessage("Moved Permanently");
    //     // res
    // }
}

std::string generateAutoIndex(const std::string &filePath, Config &config)
{
    DIR *dir;
    struct dirent *entry;
    dir = opendir(filePath.c_str());
    if (dir == NULL)
    {
        // std::cout << "Directory is =>" << filePath << std::endl;
        // std::cerr << "Error opening directory" << std::endl;
        return "";
    }
    entry = readdir(dir);
    // std::cout << "First => " << entry->d_name << std::endl;
    std::string title = filePath.substr(config.getRoot().size());
    std::string autoIndex = "<html><head><title>Index of " + title + "</title></head><body><h1>Index of " + title + "</h1><hr><ul>";
    while (entry != NULL)
    {
        std::string str(entry->d_name);
        std::string href = filePath.substr(config.getRoot().size());
        if (href[href.size() - 1] != '/')
            href += "/";
        href += str;
        autoIndex += "<pre><li><a href=\"" + href + "\">" + str + "</a></li></pre>";
        // std::cout << "filePath => " << href << std::endl;
        entry = readdir(dir);
    }
    closedir(dir);
    autoIndex += "</ul></body></html>";
    return autoIndex;
}

std::string getStatusMessage(int statusCode)
{
    switch (statusCode)
    {
    case 200:
        return "OK";
    case 201:
        return "Created";
    case 202:
        return "Accepted";
    case 301:
        return "Moved Permanently";
    case 302:
        return "Found";
    case 400:
        return "Bad Request";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 408:
        return "Request Timeout";
    case 413:
        return "Payload Too Large";
    case 414:
        return "URI Too Long";
    case 500:
        return "Internal Server Error";
    case 501:
        return "Not Implemented";
    case 502:
        return "Bad Gateway";
    default:
        return "";
    }
}

void setClientConfig(Client &client, Config &config)
{
    client.setMethod(client.getMethod());
    client.setUri(client.getUri());
    client.setVersion(client.getVersion());
    if (config.isLocation(client.getUri(), client.getPort()))
    {
		std::cout << "Location found\n";
        config.setRoot(1, client.getUri(), client.getPort());
        config.defineErrorPage(1, client.getUri(), client.getPort());
        config.setServerName(1, client.getUri(), client.getPort());
        config.setErrorPage(1, client.getUri(), client.getPort());
        config.setIndex(1, client.getUri(), client.getPort());
        config.setAutoIndex(1, client.getUri(), client.getPort());
        config.setClientMaxBodySize(1, client.getUri(), client.getPort());
        config.setUploadDir(1, client.getUri(), client.getPort());
        config.setAllowedMethods(1, client.getUri(), client.getPort());
        // config.setRedirect(1, client.getUri(), client.getPort());
    }
    else
    {
		std::cout << "Location NOT found\n";
        config.setRoot(0, "", client.getPort());
        config.defineErrorPage(0, "", client.getPort());
        config.setServerName(0, "", client.getPort());
        config.setErrorPage(0, "", client.getPort());
        config.setIndex(0, "", client.getPort());
        config.setAutoIndex(0, "", client.getPort());
        config.setClientMaxBodySize(0, "", client.getPort());
        config.setUploadDir(0, "", client.getPort());
        config.setAllowedMethods(0, "", client.getPort());
        // config.setRedirect(0, "", client.getPort());
    }
}

std::string handleRequest(Client &client, Config &config, std::string &request)
{
    Response response;
    struct stat fileStat;
    Default def;

    int fdfile = open("/tmp/tmp.txt", O_CREAT | O_RDWR, 0777);
    Cgi cgi = Cgi(fdfile, request);
    
    setClientConfig(client, config);
    std::map<std::string, int> allowedMethods = config.getAllowedMethods();
    std::string root = config.getRoot();
    std::string uri = client.getUri();
    std::string index = config.getIndex();
    std::string filePath = root + uri.substr(0, uri.size() - 1);
    std::string filePathCpy = filePath;
    std::string errorCode;
    std::string errorPage;
    // int statusMessage = 0;

    std::map<std::string, std::string> isErrorPage = config.getIsErrorPage();
    // for (std::map<std::string, int>::iterator it = isErrorPage.begin(); it != isErrorPage.end(); it++)
    // {
        
    // }
    if (config.getErrorPage().size() > 1)
    {
        errorCode = config.getErrorPage()[0];
        errorPage = config.getErrorPage()[1];
        int statusMessage;
        std::istringstream(errorCode) >> statusMessage;
    }

    // std::cout << "Port => " << client.getPort() << std::endl;
	// std::cout << "Root => " << root << std::endl;
	// std::cout << "autoIndex => " << config.getAutoIndex() << std::endl;
	// std::cout << "index => " << index << std::endl;
	// std::cout << "Errorcode => " << errorCode << std::endl;
	// std::cout << "uploadDir => " << config.getUploadDir() << std::endl;
	// std::cout << "serverName => " << config.getServerName() << std::endl;
    // if (config.getRedirect().size())
    //     std::cout << "Redirect from => " << config.getRedirect()[0] << " Redirect to => " << config.getRedirect()[1] << std::endl;
    // std::map<std::string, std::string> headers = client.getHeadersmap();
    // std::cout << "Printing Headers\n";
    // for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
    // {
    //     std::cout << it->first << " => " << it->second << std::endl;
    // }
    
	// std::cout << "AllowedMethods => " << allowedMethods.size() << std::endl;
    // for (std::map<std::string, int>::iterator it = allowedMethods.begin(); it != allowedMethods.end(); it++)
    // {
    //     std::cout << it->first << " => " << it->second << std::endl;
    // }

    if (client.getMethod() == "GET" && allowedMethods["GET"])
    {
        if (client.getUri().find("/CGIscripts/get.py") != std::string::npos)
        {
            cgi.getCgi(client);
            generateResponse(config, response, "./responsegetCGI.html", "text/html", "200", "OK", 0);
            return getResponse(response);
        }
        else
        {
            if (stat(filePath.c_str(), &fileStat) == 0) // Check if file/directory exists
            {
                if (S_ISDIR(fileStat.st_mode)) // Handle directories
                {
                    filePathCpy = root + uri + "/" + index;
                    if (access(filePathCpy.c_str(), F_OK) == 0) // File exists
                    {
                        if (access(filePathCpy.c_str(), R_OK) == 0) // File exists  + readable, serve it
                        {
                            generateResponse(config, response, filePathCpy, config.getContentType(getFileExtension(filePathCpy)), "200", "OK", 0);
                            return getResponse(response);
                        }
                        if (isErrorPage.find("403") != isErrorPage.end())
                        {
                            generateResponse(config, response, isErrorPage["403"], "text/html", "403", "Forbidden", 0);
                            return getResponse(response);
                        }
                        generateResponse(config, response, def.generateErrorPage("403"), "text/html", "403", "Forbidden", 1);
                        return getResponse(response);
                    }
                    if (config.getAutoIndex() == "on") // AutoIndex is on
                    {
                        std::string autoIndex = generateAutoIndex(filePath, config);
                        generateResponse(config, response, autoIndex, "text/html", "200", "OK", 1);
                        return getResponse(response);
                    }
                    else // AutoIndex is off
                    {
                        if (isErrorPage.find("404") != isErrorPage.end())
                        {
                            generateResponse(config, response, isErrorPage["404"], "text/html", "404", "Not Found", 0);
                            return getResponse(response);
                        }
                        else
                        {
                            generateResponse(config, response, def.generateErrorPage("404"), "text/html", "404", "Not Found", 1);
                            return getResponse(response);
                        }
                    }
                }
                else // Handle files
                {
                    generateResponse(config, response, filePathCpy, config.getContentType(getFileExtension(filePathCpy)), "200", "OK", 0);
                    return getResponse(response);
                }
            }
            else // File/Directory doesn't exist
            {
                if (isErrorPage.find("404") != isErrorPage.end())
                {
                    generateResponse(config, response, isErrorPage["404"], "text/html", "404", "Not Found", 0);
                    return getResponse(response);
                }
                generateResponse(config, response, def.generateErrorPage("404"), "text/html", "404", "Not Found", 1);
                return getResponse(response);
            }
        }
    }
    else if (client.getMethod() == "DELETE" && allowedMethods["DELETE"])
    {
        if (stat(filePath.c_str(), &fileStat) == 0) // check if file/directory exists
        {
            if (access(filePath.c_str(), F_OK | X_OK) == 0) // if file exists + executable, delete it!
            {
                if (std::remove(filePath.c_str()) != 0)
                {
                    if (isErrorPage.find("500") != isErrorPage.end())
                    {
                        generateResponse(config, response, isErrorPage["500"], "text/html", "500", "Internal Server Error", 0);
                        return getResponse(response);
                    }
                    generateResponse(config, response, "./assets/error/500.html", "text/html", "500", "Internal Server Error", 0);
                    return getResponse(response);
                }
                else
                {
                    generateResponse(config, response, "", "text/html", "204", "No Content", 1);
                    return getResponse(response);
                }
            }
            else
            {
                if (isErrorPage.find("403") != isErrorPage.end())
                {
                    generateResponse(config, response, isErrorPage["403"], "text/html", "403", "Forbidden", 0);
                    return getResponse(response);
                }
                generateResponse(config, response, def.generateErrorPage("403"), "text/html", "403", "Forbidden", 1);
                return getResponse(response);
            }
        }
        else
        {
            if (isErrorPage.find("404") != isErrorPage.end())
            {
                generateResponse(config, response, isErrorPage["404"], "text/html", "404", "Not Found", 0);
                return getResponse(response);
            }
            generateResponse(config, response, def.generateErrorPage("404"), "text/html", "404", "Not Found", 1);
            return getResponse(response);
        }
    }
    else if (client.getMethod() == "POST" && allowedMethods["POST"])
    {
        cgi.postCgi(client, config);
        generateResponse(config, response, "./responsepostCGI.html", "text/html", "200", "OK", 0);
        return getResponse(response);
    }
    else
    {
        if (isErrorPage.find("405") != isErrorPage.end())
        {
            generateResponse(config, response, isErrorPage["405"], "text/html", "405", "Method Not Allowed", 0);
            return getResponse(response);
        }
        generateResponse(config, response, def.generateErrorPage("405"), "text/html", "405", "Method Not Allowed", 1);
        return getResponse(response);
    }
    return "";
}