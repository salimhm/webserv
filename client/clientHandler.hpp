/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 02:07:38 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/15 21:42:04 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Client.hpp"
#include "../utils.hpp"
#include "../parsing/Mime.hpp"

Request parseRequest(const std::string &request);
std::string handleRequest(Client& client, const Config& config);