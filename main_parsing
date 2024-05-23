/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 21:35:35 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/21 09:57:51 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/Parser.hpp"

int main()
{
    std::string filePath = "webserv.yml";
    try
    {
        Parser parser(filePath);
        parser.parse();

        std::vector<std::pair<std::string, std::vector<std::string> > > globalDirectives = parser.getGlobalDirectives();
        std::vector<std::pair<std::string, std::pair<std::string, std::vector<std::string> > > > locations = parser.getLocations();

        std::cout << "************* GLOBAL DIRECTIVES **************\n";
        for(size_t i = 0; i < globalDirectives.size(); i++)
        {
            std::cout << globalDirectives[i].first << " ==> " << globalDirectives[i].second[0] << std::endl;
        }
        
        std::cout << "************* LOCAL DIRECTIVES **************\n";
        // std::cout << locations.size() << std::endl;
        for (size_t i = 0; i < locations.size(); i++)
        {
            std::cout << "-------------- " << locations[i].first << " --------------" << std::endl;
            for (size_t j = 0; j < locations[i].second.second.size(); j++)
            {
                std::cout << locations[i].second.first << " ==> " << locations[i].second.second[j] << std::endl;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}