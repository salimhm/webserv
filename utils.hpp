/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shmimi <shmimi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:17:14 by shmimi            #+#    #+#             */
/*   Updated: 2024/06/05 20:53:48 by shmimi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server/Server.hpp"

std::vector<std::string> split(const std::string &toSplit, std::string delimiter);
std::string trim(const std::string &str);
std::string normalizeUrl(const std::string &uri);