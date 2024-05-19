/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 21:35:35 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/18 19:25:00 by shmimi           ###   ########.fr       */
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
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}