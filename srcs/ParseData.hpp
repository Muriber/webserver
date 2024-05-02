/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseData.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjimenez <bjimenez@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 12:59:37 by bjimenez          #+#    #+#             */
/*   Updated: 2023/12/05 14:04:03 by bjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PARSEDATA_HPP
#define	PARSEDATA_HPP

#include <iostream>
#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include "CFGcluster.hpp"

class ParseData
{
private:
	std::string							request;
	std::string							root;
	std::map<std::string, std::string>	data_resp;
	std::string							v_post;
	bool								endPost;
	
public:
	ParseData(void);
	ParseData(std::string);
	ParseData(ParseData &);
	ParseData&	operator=(ParseData &);
	~ParseData();

	void			check_endPost(void);
	void			get_data_resp(void);
	bool			get_data_value(int, std::string);
	std::string		get_data(std::string);
	bool			get_endPost(void);

};

#endif