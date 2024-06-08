/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 02:07:38 by shmimi            #+#    #+#             */
/*   Updated: 2024/06/08 21:07:27 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"
#include "../utils.hpp"
#include "../parsing/Mime.hpp"
#include "../Cgi/Cgi.hpp"

std::string parseRequest(Client& client, const std::string &request);
std::string handleRequest(Client& client, Config& config, std::string &request);
std::string readFile(std::string filePath);
int getHeaders(const std::string& request, Client& client);
