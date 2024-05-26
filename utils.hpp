/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:17:14 by shmimi            #+#    #+#             */
/*   Updated: 2024/05/26 10:56:25 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server/Server.hpp"

std::vector<std::string> split(const std::string &toSplit, std::string delimiter);
std::string trim(std::string &str);
std::string normalizeUrl(std::string &uri);