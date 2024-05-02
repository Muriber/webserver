/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjimenez <bjimenez@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 08:32:49 by bjimenez          #+#    #+#             */
/*   Updated: 2023/12/05 14:21:55 by bjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <poll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "Response.hpp"
#include "SrvConfigFile.hpp"
#include "CFGcluster.hpp"
#include "Response.hpp"
#include <vector>
#include "ParseData.hpp"

class Server
{
	private:
        int								new_socket;
		int								n_fd;
		std::string						ip_dir;
		std::vector<int>				server_socket;
		std::vector<int>				port;
		std::vector<std::string>		root;
		std::vector<struct sockaddr_in>	server_socket_add;
	    std::vector<socklen_t>			server_socket_add_len;
		std::map<int, std::vector<char> >				client_msg;
		std::vector<std::string>		client_rsp;
		std::vector<bool>				expect_continue;
		std::vector<int>				fd_socket;
		struct pollfd					poll_fd[150];
		Response						*resp;
		SrvConfigFile					*cfg;
		bool							resp_status;
		std::vector<std::string>		mensaje;

	public:
		Server(void);
		Server(SrvConfigFile &);
		Server(Server const &copy);
		Server&	operator=(Server const &obj);
		~Server();
		
		CFGcluster						*ptr;

		int								init_socket(void);
		void							close_socket(void);
		void							init_listen(void);
        int								accept_com(int);
		void							poll_funct(void);
		void							response_get(int);
		void							response_post(int);
		void							response_delete(int);
		void							newanswer(int, int, int);
		struct pollfd					Init_poll_servers(int);
		void							reset_fds(int, std::vector<int>&);
		void							pollout_process(int);
		bool							pollin_process(int, std::vector<int>&);
		bool							pollin_addclient(int &, std::vector<int>&);
		void							pollin_get_request(int, std::vector<int>&);
		void							check_post_request(int, std::vector<int>&);
		void							init_sockaddr(int);
		int								set_socket_add(int);
};

class ErrorServerException: public std::exception
{
	private:
		std::string _error;
	public:
		ErrorServerException(const std::string error_code)
		{
			_error = error_code;
		}
		virtual const char* what() const _NOEXCEPT
		{
			return (_error.c_str());
		}
		virtual ~ErrorServerException() _NOEXCEPT
		{
		
		}
};

#endif