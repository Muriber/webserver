/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjimenez <bjimenez@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 12:59:37 by bjimenez          #+#    #+#             */
/*   Updated: 2023/12/05 14:16:01 by bjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	RESPONSE_HPP
#define	RESPONSE_HPP

#include <iostream>
#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <dirent.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include "CFGcluster.hpp"
#include "CGIexec.hpp"
#include <sys/stat.h>

class Response
{
	private:
		std::string							request;
		std::string							root;
		std::string							v_post;
		std::string							cod;
		std::string							est;
		std::string							index;
		std::string							type_path;
		std::string							cgi_type;
		std::map<std::string, std::string>	data_resp;
		std::map<std::string, std::string>	data_post;
		std::string							dir_upload;
		CFGcluster							*SrvData;

		std::string							file_type;
		
	public:
		Response(void);
		Response(std::string, std::string, CFGcluster&);
		Response(Response &);
		Response& operator=(Response &);
		~Response();

		std::string		check_data_resp(void);
		std::string		Rsp_get(void);
		std::string		Rsp_post(void);
		std::string		Rsp_delete(void);
		std::string		Rsp_excep(std::string);
		std::string		check_type(void);
		void			eval_get_index(std::ifstream &);
		void			clean_multipost(void);
		std::string		generate_response(void);
		std::string		get_data_resp(void);
		bool			get_data_value(std::string);
		void			get_postdata(void);
		void			get_index(void);
		void			get_url(void);
		void			generate_index(std::string);
		bool			cgi_option(void);
		void			check_get_vrt_host(void);
		void			check_del_vrt_host(void);
		
		void			define_virtualHost(void);

		void			get_content_type(void);
};

class ErrorResponseException: public std::exception
{
	private:
		std::string _error;
	public:
		ErrorResponseException(const std::string error_code)
		{
			_error = error_code;
		}
		virtual const char* what() const _NOEXCEPT
		{
			return (_error.c_str());
		}
		virtual ~ErrorResponseException() _NOEXCEPT
		{
		
		}
};

#endif