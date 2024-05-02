/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseData.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjimenez <bjimenez@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 13:06:10 by bjimenez          #+#    #+#             */
/*   Updated: 2023/12/05 14:01:12 by bjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseData.hpp"

ParseData::ParseData(void)
{
	this->request.clear();
	this->root.clear();
	this->data_resp.clear();
	this->v_post.clear();
	this->endPost = false;
}

ParseData::ParseData(std::string input)
{
	this->request = input;
	this->endPost = false;
	ParseData::get_data_resp();
}

ParseData::ParseData(ParseData &copy)
{
	this->request = copy.request;
	this->endPost = copy.endPost;
	this->root = copy.root;
	this->data_resp = copy.data_resp;
	this->v_post = copy.v_post;
	this->endPost = copy.endPost;
}

ParseData& ParseData::operator=(ParseData &obj)
{
	this->request = obj.request;
	this->endPost = obj.endPost;
	this->root = obj.root;
	this->data_resp = obj.data_resp;
	this->v_post = obj.v_post;
	this->endPost = obj.endPost;
	return (*this);
}

ParseData::~ParseData(void)
{
}

bool	ParseData::get_data_value(int opt, std::string aux)
{
	switch (opt)
	{
		case 1:
			if (aux.substr(0, aux.find(':')) == "Host")
			{
				this->data_resp["Host"] = aux.substr(aux.find(':') + 2);
				return (true);
			}
		case 2:
			if (aux.substr(0, aux.find(':')) == "Connection")
			{
				this->data_resp["Connection"] = aux.substr(aux.find(':') + 2);
				return (true);
			}
		case 3:
			if (aux.substr(0, aux.find(':')) == "User-Agent")
			{
				this->data_resp["User-Agent"] = aux.substr(aux.find(':') + 2);
				return (true);
			}
		case 4:
			if (aux.substr(0, aux.find(':')) == "Accept")
			{
				this->data_resp["Accept"] = aux.substr(aux.find(':') + 2);
				return (true);
			}
		case 5:
			if (aux.substr(0, aux.find(':')) == "Referer")
			{
				this->data_resp["Referer"] = aux.substr(aux.find(':') + 2);
				return (true);
			}
		case 6:
			if (aux.substr(0, aux.find(':')) == "Accept-Encoding")
			{
				this->data_resp["Accept-Encoding"] = aux.substr(aux.find(':') + 2);
				return (true);
			}
		case 7:
			if (aux.substr(0, aux.find(':')) == "Content-Length")
			{
				this->data_resp["Content-Length"] = aux.substr(aux.find(':') + 2);
				return (true);
			}
		case 8:
			if (aux.substr(0, aux.find(':')) == "Upgrade-Insecure-Requests")
			{
				this->data_resp["Upgrade-Insecure-Requests"] = aux.substr(aux.find(':') + 2);
				return (true);
			}
		case 9:
			if (aux.substr(0, aux.find(':')) == "Origin")
			{
				this->data_resp["Origin"] = aux.substr(aux.find(':') + 2);
				return (true);
			}
		case 10:
			if (aux.substr(0, aux.find(':')) == "Content-Type")
			{
				this->data_resp["Content-Type"] = aux.substr(aux.find(':') + 2);
				if (this->data_resp["Content-Type"].find("multipart") != std::string::npos)
				{
					this->data_resp["boundary"] = aux.substr(aux.find('=') + 1);
					this->data_resp["end_bound"] = this->data_resp["boundary"].substr(0, this->data_resp["boundary"].length()) + "--";
				}
				return (true);
			}
		default:
			return (false);
	}
	return (false);
}

void	ParseData::check_endPost(void)
{
	if (v_post.length() == (unsigned long)atol(this->data_resp["Content-Length"].c_str()))
		this->endPost = true;
}

void	ParseData::get_data_resp(void)
{
	std::string	aux;
	std::string line(this->request);
	int 		i = 1;

	aux = this->request.substr(0, this->request.find('\n') - 1);
	this->data_resp["type"] = aux.substr(0, aux.find(' '));
	this->data_resp["url"] = aux.substr(aux.find(' ') + 1, aux.rfind(' ') - aux.find(' ') - 1);
	if (this->data_resp["url"].rfind('/') == this->data_resp["url"].length() - 1)
		this->data_resp["url"] = this->data_resp["url"].erase(this->data_resp["url"].rfind('/'), 1);
	this->data_resp["protoc"] = aux.substr(aux.rfind(" ") + 1);
	
	std::istringstream input(this->request);
	
	for (; std::getline(input, aux, '\n'); )
	{
		if (aux.find('\r') != std::string::npos)
			aux = aux.erase(aux.find('\r'), 1);
 		if (ParseData::get_data_value(i, aux))
			i++;
	}
	this->v_post.clear();
	size_t pos  = line.find("\r\n\r\n");
	if (pos != std::string::npos)
		this->v_post = line.substr(pos + 4);
	ParseData::check_endPost();
}

/**********GETS*************/

std::string		ParseData::get_data(std::string data)
{
	return(this->data_resp[data]);
}

bool			ParseData::get_endPost(void)
{
	return(this->endPost);
}