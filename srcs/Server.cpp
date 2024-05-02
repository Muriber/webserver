/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjimenez <bjimenez@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 08:36:59 by bjimenez          #+#    #+#             */
/*   Updated: 2023/12/11 08:24:18 by bjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void)
{
	this->new_socket = 0;
	this->n_fd = 0;
	this->ip_dir.clear();
	this->server_socket.clear();
	this->port.clear();
	this->root.clear();
	this->server_socket_add.clear();
	this->server_socket_add_len.clear();
	this->client_msg.clear();
	this->client_rsp.clear();
	this->expect_continue.clear();
	this->fd_socket.clear();
	this->resp = NULL;
	this->cfg = NULL;
	this->resp_status = false;
	this->mensaje.clear();	
}

Server::Server(Server const &copy)
{
	this->new_socket = copy.new_socket;
	this->n_fd = copy.n_fd;
	this->ip_dir = copy.ip_dir;
	this->server_socket = copy.server_socket;
	this->port = copy.port;
	this->root = copy.root;
	this->server_socket_add = copy.server_socket_add;
	this->server_socket_add_len = copy.server_socket_add_len;
	this->client_msg = copy.client_msg;
	this->client_rsp = copy.client_rsp;
	this->expect_continue = copy.expect_continue;
	this->fd_socket = copy.fd_socket;
	this->resp = copy.resp;
	this->cfg = copy.cfg;
	this->resp_status = copy.resp_status;
	this->mensaje = copy.mensaje;	
}

Server&	Server::operator=(Server const &obj)
{
	this->new_socket = obj.new_socket;
	this->n_fd = obj.n_fd;
	this->ip_dir = obj.ip_dir;
	this->server_socket = obj.server_socket;
	this->port = obj.port;
	this->root = obj.root;
	this->server_socket_add = obj.server_socket_add;
	this->server_socket_add_len = obj.server_socket_add_len;
	this->client_msg = obj.client_msg;
	this->client_rsp = obj.client_rsp;
	this->expect_continue = obj.expect_continue;
	this->fd_socket = obj.fd_socket;
	this->resp = obj.resp;
	this->cfg = obj.cfg;
	this->resp_status = obj.resp_status;
	this->mensaje = obj.mensaje;
	return (*this);
}

Server::Server(SrvConfigFile &cfg)
{
	this->port.clear();
	this->root.clear();
	this->resp_status = false;

	this->cfg = &cfg;
	for (int i = 0; i < this->cfg->getnbrSrv(); i++)
	{
		this->port.push_back(this->cfg->SrvCfg[i].getListen());//
		this->root.push_back(this->cfg->SrvCfg[i].getRoot());//
		this->server_socket.push_back(-1);
	}
	for (int i = 0; i < 150; i++)
	{
		this->mensaje.push_back("");
		this->client_rsp.push_back("");
		this->fd_socket.push_back(-1);
	}	
	this->new_socket = -1;
	this->n_fd = this->cfg->getnbrSrv();
	std::vector<bool> expecto(150, true);
	this->expect_continue = expecto;
}

Server::~Server()
{
	std::vector<int>::iterator	it;

	it = server_socket.begin();
	while(it != server_socket.end())
	{
		close(*it);
		it++;
	}
	close_socket();	
}

int	Server::init_socket(void)
{
	this->server_socket.clear();
	for (int i = 0; i < this->cfg->getnbrSrv(); i++)
	{
		this->server_socket.push_back(socket(AF_INET, SOCK_STREAM, 0));
		if (this->server_socket.back() < 0)
		{	
			std::cerr<<"---->Error Socket: Cannot be created"<<std::endl;
			return (1);
		}
	}
		for (int i = 0; i < this->cfg->getnbrSrv(); i++)
	{
		Server::init_sockaddr(i);
		if (Server::set_socket_add(this->server_socket[i]) < 0)
			return (1);
		if ( bind(this->server_socket[i], (struct sockaddr *)&this->server_socket_add[i], this->server_socket_add_len[i]) != 0)
		{
			std::cerr<<"---->Error Socket: Cannot connect socket to address..."<<std::endl;
			return (1);
		}
		std::cout<<".: Init "<<this->cfg->SrvCfg[i].getSrvNme()<<" in Socket "<<this->server_socket[i]<<" at port:"<<this->port[i]<<" Buffer:"<<this->cfg->SrvCfg[i].getBodyBuffer()<<std::endl;
	}
	return (0);
}

void	Server::init_sockaddr(int i)
{
 		this->server_socket_add.push_back(sockaddr_in());
		bzero(&(this->server_socket_add.back()), sizeof(struct sockaddr_in));
		this->server_socket_add.back().sin_family = AF_INET;				
		this->server_socket_add.back().sin_port = htons(this->port[i]);
		this->server_socket_add.back().sin_addr.s_addr = INADDR_ANY;
		this->server_socket_add_len.push_back(socklen_t());
		this->server_socket_add_len.back() = sizeof(this->server_socket_add.back());
		bzero(this->server_socket_add.back().sin_zero, 8);
}

int		Server::set_socket_add(int socket)
{
	int	option_value = 1;
	
	int re_add = setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));
	int re_port = setsockopt(socket, SOL_SOCKET, SO_REUSEPORT, &option_value, sizeof(option_value));
	if (re_add < 0 || re_port)
		std::cerr<<"---->Error setsockopt"<<std::endl;
	return (std::min(re_add, re_port));
}

void	Server::close_socket(void)
{
	for (int i = 0; i < 150; i++)
		close(this->fd_socket[i]);
}

void	Server::init_listen(void)
{	
	for (int i = 0; i < this->cfg->getnbrSrv(); i++)
	{
		if (listen(this->server_socket[i], 10) < 0 && i < this->cfg->getnbrSrv())
		{
			std::cerr<<"---->Error socket: listen failed "<<this->server_socket[i]<<std::endl;
			exit(1);
		}
	
	std::cout<<"-------->Listening on Port: "<<ntohs(this->server_socket_add[i].sin_port)<<std::endl;
	std::cout<<"-------->Waiting to client...\n"<<std::endl;
	}
	Server::poll_funct();
}

/********************************/
/*				POLL 			*/
/********************************/

void	Server::poll_funct(void)
{
	std::vector<int>	n_server(150, -1);

	memset(this->poll_fd, 0, sizeof(this->poll_fd));

	for (int i = 0; i < this->cfg->getnbrSrv(); i++)
		this->poll_fd[i] = Server::Init_poll_servers(i);
	do
	{
		int nbr_fd_ready = poll(this->poll_fd, this->n_fd, -1);
		if (nbr_fd_ready < 0)
		{
			std::cerr<<"Error: poll creation..."<<std::endl;
			exit(1);
		}
			
		int size = this->n_fd;
		for (int i = 0; i < size; i++)
		{
			if (nbr_fd_ready == 0 && i >= this->cfg->getnbrSrv())
			{
				this->poll_fd[i].fd = -1;
				this->fd_socket[i] = -1;
			}	
			if (this->poll_fd[i].revents == 0)
				continue;
			else if (i < this->cfg->getnbrSrv() && !(this->poll_fd[i].revents & POLLIN))
			{
				std::cerr<<"Error Server: fd "<<this->poll_fd[i].fd<<" revents: "<<this->poll_fd[i].revents<<std::endl;
				close(this->poll_fd[i].fd);
				this->poll_fd[i].fd = -1;
				this->fd_socket[i] = -1;
				break;
			}
			else if (i >= this->cfg->getnbrSrv() && !(this->poll_fd[i].revents & POLLIN) && !(this->poll_fd[i].revents & POLLOUT))
			{
				std::cerr<<"Error Client: fd "<<this->poll_fd[i].fd<<" revents: "<<this->poll_fd[i].revents<<std::endl;
				close(this->fd_socket[i]);
				this->fd_socket[i] = -1;
				this->poll_fd[i].fd = -1;
				break;
			}
			if (this->poll_fd[i].revents & POLLIN)
			{
				if (!Server::pollin_process(i, n_server))
					break;
			}
			else if ((this->poll_fd[i].revents & POLLOUT) && this->resp_status)
				Server::pollout_process(i);
		}
		Server::reset_fds(size, n_server);
	}while(true);
}

/********************************/
/*			POLL UTILS			*/
/********************************/

size_t	sendtoclient(int fd, std::string resp)
{
	size_t send_bytes = write(fd, resp.c_str(), resp.length());
	return (send_bytes);
}

struct pollfd	Server::Init_poll_servers(int i)
{
	struct pollfd	poll_fd;

	if (int n_bloc = fcntl(this->server_socket[i], F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr<<"Error: fcntl function..."<<n_bloc<<std::endl;
		exit(1);
	}
	poll_fd.events = POLLIN;
	poll_fd.fd = this->server_socket[i];
	this->fd_socket[i] = this->server_socket[i];
	return (poll_fd);
}

int	Server::accept_com(int i)
{
    this->new_socket = accept(this->server_socket[i], (sockaddr *)&this->server_socket_add[i], &this->server_socket_add_len[i]);
	
	if (this->new_socket < 0)
    {
        std::cerr<<"---->Listening Finished..."<<errno<<std::endl;
		return (-1);
    }
	if (int n_bloc = fcntl(this->new_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr<<"Error: fcntl function..."<<n_bloc<<std::endl;
		exit(1);
	}
	int option_value;
	int reg = setsockopt(this->new_socket, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));
	if (reg < 0)
	{
		std::cerr<<"---->Error setsockopt"<<std::endl;
		return (-1);
	}
	std::cout<<"-------->Communication accepted"<<std::endl;
	return (i);
}

bool	Server::pollin_addclient(int &i, std::vector<int> &n_server)
{
	int server_idx = Server::accept_com(i);
	if (server_idx == -1)
		return (false);
	while (i < 150 && this->fd_socket[i] != -1)
		i++;
	this->poll_fd[i].fd = this->new_socket;
	this->fd_socket[i] = this->new_socket;
	this->poll_fd[i].events = POLLIN;
	n_server[i] = server_idx;
	this->n_fd++;
	return (true);
}

void	Server::pollin_get_request(int i, std::vector<int> &n_server)
{
	char	*buffer = new char[this->cfg->SrvCfg[n_server[i]].getBodyBuffer()];

	ssize_t 	recep = read(this->poll_fd[i].fd, buffer, this->cfg->SrvCfg[n_server[i]].getBodyBuffer());
	
	if (recep <= 0)
	{
		std::cerr<<"----> No data to read from Client FD:"<<this->poll_fd[i].fd<<"...recep: "<<recep<<std::endl;
		close(this->fd_socket[i]);
		this->poll_fd[i].fd = -1;
		this->fd_socket[i] = -1;
	}
	else
	{
		this->client_msg[i].insert(this->client_msg[i].end(), buffer, buffer + recep);
		this->mensaje[i] = std::string(this->client_msg[i].begin(), this->client_msg[i].end());
		if (this->mensaje[i].find("Expect: 100-continue") != std::string::npos && this->expect_continue[i])
		{
			size_t bytes = sendtoclient(this->fd_socket[i], "HTTP/1.1 100 Continue\n");
			if (bytes <= 0)
				std::cerr<<"---->Error sending message to CLIENT..."<<bytes<<std::endl;
			this->expect_continue[i] = false;
		}
		if (this->mensaje[i].find("Content-Length") != std::string::npos)
			Server::check_post_request(i, n_server);
		else if (this->mensaje[i].substr(this->mensaje[i].length() - 4) == "\r\n\r\n" || (this->mensaje[i].find("\r\n\r\n") != std::string::npos))
			Server::newanswer(this->poll_fd[i].fd, i, n_server[i]);
	}
	delete [] buffer;
}

void	Server::check_post_request(int i,std::vector<int> &n_server)
{
	ParseData datos(this->mensaje[i]);

	if (datos.get_endPost() && this->mensaje[i].find("multipart") == std::string::npos)
		Server::newanswer(this->poll_fd[i].fd, i, n_server[i]);
	else if (this->mensaje[i].find("multipart") != std::string::npos && this->mensaje[i].find(datos.get_data("end_bound")) != std::string::npos)
		Server::newanswer(this->poll_fd[i].fd, i, n_server[i]);
}

bool	Server::pollin_process(int i, std::vector<int> &n_server)
{
	if (i < this->cfg->getnbrSrv() && this->n_fd < 150)
	{
		if (!Server::pollin_addclient(i, n_server))
			return (false);
	}
	else
		Server::pollin_get_request(i, n_server);
	return (true);
}

void	Server::pollout_process(int i)
{
	if (this->resp_status)
	{
		size_t bytes = sendtoclient(this->fd_socket[i], this->client_rsp[i]);
		if (bytes <= 0)
			std::cerr<<"---->Error sending message to CLIENT..."<<bytes<<std::endl;
		close(this->fd_socket[i]);
		this->poll_fd[i].fd = -1;
		this->fd_socket[i] = -1;
	}
}

void	Server::reset_fds(int size, std::vector<int> &server)
{
 	for (int i = 0; i < size; i++)
	{
		if (this->poll_fd[i].fd == -1)
		{
			this->poll_fd[i].fd = -1;
			this->client_msg[i].clear();
			this->client_rsp[i] = "";
			server[i] = -1;
			this->n_fd--;
		}
	}
}

void	Server::newanswer(int client_fd, int i, int n_fd_socket)
{
	(void)client_fd;
	
	Response	response(this->mensaje[i], this->root[n_fd_socket], this->cfg->SrvCfg[n_fd_socket]);
	
	this->resp = &response;
	this->client_rsp[i] = this->resp->get_data_resp();
	this->resp_status = true;
	this->poll_fd[i].events = POLLOUT;
	this->client_msg[i].clear();
	this->expect_continue[i] = true;
}
/********************************/
/*			END POLL UTILS		*/
/********************************/
