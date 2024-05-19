/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 22:48:15 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/19 21:49:53 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

enum Directives {
    LISTEN,
    SERVER_NAME,
    ROOT,
    INDEX,
    ERROR_PAGE,
    AUTO_INDEX,
    LIMIT_CLIENT_BODY_SIZE,
    UPLOAD_DIR,
    
};

class Parser
{
    private:
        std::string filePath;
        std::vector<std::pair<std::string, std::pair<std::string, std::vector<std::string> > > > locations;
        std::vector<std::pair<std::string, std::vector<std::string> > > globalDirectives;
    public:
        Parser(std::string& filePath);
        void parse();
};