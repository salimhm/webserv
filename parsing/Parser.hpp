/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 22:48:15 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/29 13:16:07 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

class Parser
{
    private:
        const std::string filePath;
        std::vector<std::string> allowedDirectives;

        std::vector<std::pair<std::string, std::pair<std::string, std::vector<std::string> > > > locations;
        std::vector<std::pair<std::string, std::vector<std::string> > > globalDirectives;
        
        std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > > allServers;
    public:
        Parser();
        ~Parser();
        Parser(const std::string& filePath);
        void parse();
        std::vector<std::pair<std::string, std::vector<std::string> > > parseServer(std::string &serverBlock);
        void checkGlobalDuplicates(const std::vector<std::string>&);
        void checkLocationsDuplicates(const std::vector<std::string>&);
        int checkSyntax(std::vector<std::pair<std::string, std::vector<std::string> > > directives);
        void setServerDirectives(std::vector<std::pair<std::string, std::vector<std::string> > > directives);
        
        //******** GETTERS *********
        std::vector<std::pair<std::string, std::pair<std::string, std::vector<std::string> > > > getLocations();
        std::vector<std::pair<std::string, std::vector<std::string> > > getGlobalDirectives();
        std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > > getAllServers();
};