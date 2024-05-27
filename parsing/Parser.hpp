/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 22:48:15 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/27 13:08:44 by shmimi           ###   ########.fr       */
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
    public:
        Parser();
        ~Parser();
        Parser(const std::string& filePath);
        void parse();
        std::vector<std::pair<std::string, std::vector<std::string> > > parseServer(std::string &serverBlock);
        int checkSyntax(std::vector<std::pair<std::string, std::vector<std::string> > > directives);

        void setServerDirectives(std::vector<std::pair<std::string, std::vector<std::string> > > directives);
        
        //******** GETTERS *********
        std::vector<std::pair<std::string, std::pair<std::string, std::vector<std::string> > > > getLocations();
        std::vector<std::pair<std::string, std::vector<std::string> > > getGlobalDirectives();
};