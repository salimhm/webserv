/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 02:07:06 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/02 00:17:12 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "clientHandler.hpp"
#include "../server/Response.hpp"
#include <sstream>

struct Request parseRequest(const std::string &request)
{
    try
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
        std::cout << "************ Body *************\n";
        std::cout << httpRequest.body << std::endl;
        std::cout << "************ END Body *************\n";
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
    }
    std::string line;
    std::string content;
    while (getline(file, line))
        content += line;
    return content;
}

std::string getResponse(Response responseObj)
{
    std::string response;
    // std::string response = "HTTP/1.1 200 OK\nContent-Type: text/text\nContent-Length: 21\n\nHelloThisisfrom lolol";

    response += responseObj.getHttpVersion() + " " + responseObj.getStatus() + " " + responseObj.getStatusMessage() + "\n" + responseObj.getContentType() + "\n" + responseObj.getContentLength() + "\n\n" + responseObj.getBody();
    return response;
}

void generateResponse(Response &response, const std::string &filePath, const std::string& contentType)
{
    response.setHttpVersion("HTTP/1.1");
    response.setStatus("200");
    response.setStatusMessage("OK");
    response.setContentType("Content-Type: " + contentType);
    response.setBody(readFile(filePath));
    std::stringstream bodyLength;
    bodyLength << response.getBody().size();
    response.setContentLength("Content-Length: " + bodyLength.str());
    response.setBody(readFile(filePath));
}

std::string handleRequest(Client client, const Mime &mime)
{
    (void)mime;

    client.setMethod(client.getMethod());
    client.setUri(client.getUri());
    client.setVersion(client.getVersion());

    Response response;

    std::string filePath = client.getRoot() + client.getUri();
    if (client.getMethod() == "GET")
    {
        // if (access(filePath.c_str(), F_OK) == 0)
        {
            if (getFileExtension(filePath).size() == 0) // Handle files without extensions
            {
                generateResponse(response, filePath, "text/plain");
                // std::cout << getResponse(response) << std::endl;
                return getResponse(response);
            }
            else
            {
                // std::cout << "Extension is =>" << getFileExtension(filePath) << std::endl;
                std::string contentType = mime.getContentType(getFileExtension(filePath));
                std::cout << "Content type is =======> " << contentType << std::endl;
                generateResponse(response, filePath, mime.getContentType(getFileExtension(filePath)));
                return getResponse(response);
            }
        }
    }
    return "";
}