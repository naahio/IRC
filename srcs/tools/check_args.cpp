/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbabela <mbabela@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 09:20:15 by mbabela           #+#    #+#             */
/*   Updated: 2022/08/22 09:29:07 by mbabela          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "tool.hpp"

bool isNumeric(std::string const &str)
{
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}