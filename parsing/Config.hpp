/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:15:53 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/27 16:25:24 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server/Server.hpp"
#include "../parsing/Mime.hpp"
#include "../parsing/Parser.hpp"

class Client;

class Config: public Mime, public Parser
{
    private:
        std::vector<int> port;
        std::string serverName;
        std::string root;
        std::string index;
        std::vector<std::string> errorPage;
        std::string autoIndex;
        std::string clientMaxBodySize;
        std::map<std::string, int> allowedMethods;
        
        const std::string& filePath;
        static std::string nullFile; //Contains nothing

        std::vector<std::pair<std::string, std::vector<std::string> > > globalDirectives;
        std::vector<std::pair<std::string, std::pair<std::string, std::vector<std::string> > > > locations;

    public:
        Config();
        Config(const std::string& file);
        
        Config(const Config& cpy);
        Config& operator=(const Config& cpy);

        // std::string getFileContent(const std::string& filename) const;
        
        /************ SETTERS ************/
        void setPort();
        void setServerName(int isLocation, const std::string& uri);
        void setRoot(int isLocation, const std::string& uri);
        void setErrorPage(int isLocation, const std::string& uri);
        void setIndex(int isLocation, const std::string& uri);
        void setAutoIndex(int isLocation, const std::string& uri);
        void setClientMaxBodySize(int isLocation, const std::string& uri);
        void setAllowedMethods(int isLocation, const std::string &uri);
        
        /************ GETTERS ************/
        const std::vector<int> getPort();
        const std::string getServerName();
        const std::string getRoot();
        const std::vector<std::string> getErrorPage();
        const std::string getIndex();
        const std::string getAutoIndex();
        const std::string getClientMaxBodySize();
        const std::map<std::string, int> getAllowedMethods();

        std::string getErrorPage(const std::string& errorCode, const std::string& uri, int location);
        std::string getErrorCode();
        int isLocation(const std::string& uri);
        
        size_t getConfigLocationIndex(const std::string &clientUri);
};