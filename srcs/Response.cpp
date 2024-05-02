/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjimenez <bjimenez@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 13:06:10 by bjimenez          #+#    #+#             */
/*   Updated: 2023/12/11 09:14:06 by bjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void)
{
	this->request.clear();
	this->root.clear();
	this->v_post.clear();
	this->cod.clear();
	this->est.clear();
	this->index.clear();
	this->type_path.clear();
	this->cgi_type.clear();
	this->data_resp.clear();
	this->data_post.clear();
	this->dir_upload.clear();
	this->SrvData = NULL;
	this->file_type.clear();
}

Response::Response(std::string input, std::string root, CFGcluster &SrvData)
{
	
	this->request = input;
	this->root = root;
	this->SrvData = &SrvData;
	this->cod ="";
	this->index = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n	<meta charset=\"UTF-8\">\n	<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n		<title>ElYogurServer</title>\n</head>\n<body>\n	<h1 style=\"text-align:center\">Hello From Index " + this->SrvData->getSrvNme() + "</h1>\n";
	this->file_type = "text/html";
	
	CFGlocation* upload = this->SrvData->getLocation("upload");
	if (upload == NULL)
		this->dir_upload = this->root;
	else 
		this->dir_upload = upload->getUploadStore();
}

Response::Response(Response &copy)
{
	this->request = copy.request;
	this->root = copy.root;
	this->v_post = copy.v_post;
	this->cod = copy.cod;
	this->est = copy.est;
	this->index = copy.index;
	this->type_path = copy.type_path;
	this->cgi_type = copy.cgi_type;
	this->data_resp = copy.data_resp;
	this->data_post = copy.data_post;
	this->dir_upload = copy.dir_upload;
	this->SrvData = copy.SrvData;
	this->file_type = copy.file_type;
}

Response& Response::operator=(Response &obj)
{
	this->request = obj.request;
	this->root = obj.root;
	this->v_post = obj.v_post;
	this->cod = obj.cod;
	this->est = obj.est;
	this->index = obj.index;
	this->type_path = obj.type_path;
	this->cgi_type = obj.cgi_type;
	this->data_resp = obj.data_resp;
	this->data_post = obj.data_post;
	this->dir_upload = obj.dir_upload;
	this->SrvData = obj.SrvData;
	this->file_type = obj.file_type;
	return (*this);
}

Response::~Response(void)
{
}

bool	Response::get_data_value(std::string aux)
{
	for(int opt = 1; opt < 12; opt++)
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
						this->data_resp["boundary"] = aux.substr(aux.find('=') + 1);
					return (true);
				}
			case 11:
				if (aux.substr(0, aux.find(':')) == "Content-Disposition")
				{
					this->data_resp["filename"] = aux.substr(aux.rfind('=') + 2, (aux.rfind('\"') - aux.rfind('=') - 2));
					std::cout<<".:Filename:"<<this->data_resp["filename"]<<std::endl;
					return (true);
				}
			default:
				return (false);
		}
	}
	return (false);
}

std::string	get_date(void)
{
    std::time_t time = std::time(nullptr);
    std::tm* now = std::localtime(&time);
 
    char data[128];
    strftime(data, sizeof(data), "%m-%d-%Y %X", now);
    return (data);
}

std::string	find_route(std::string input)
{
	int	pos = 0;
	
	for (int i = 0; i < 3; i++)
	{
		pos = input.find("/", pos);
		pos++;
	}
	return(input.substr(pos, input.length()));
}

void	Response::generate_index(std::string route)
{
	DIR	*dir;
	struct dirent		*dir_list;
	std::vector<char *>	files;

	char				pwd[150];
	
	if ((dir = opendir(((std::string)getcwd(pwd, 100) + "/" + route).c_str())) != nullptr)
	{
		dir_list = readdir(dir);
		while(dir_list != nullptr)
		{
			files.push_back(dir_list->d_name);
			dir_list = readdir(dir);
		}
		closedir(dir);
	}
 	for (size_t i = 0; i < files.size(); i++)
		this->index.append("\t<p>" + route +":<a href=\"" + (std::string)files[i]
		+ "/\">" +(std::string)files[i] + "</a>.</p>\n");
	this->index.append("</body>\n</html>");
}

void		Response::eval_get_index(std::ifstream &i_file)
{
	i_file.open(this->root + this->SrvData->getIndex(), std::ifstream::binary);
	
	if (!i_file.is_open() && this->SrvData->getAutoindex() == "off")
	{
		std::cerr<<"Error. No such file: "<<this->root + this->SrvData->getIndex()<<std::endl;
		this->cod = "Error. No such file: " + this->root + this->SrvData->getIndex();
		this->est =  " 204 No Content";
	}
	else if (!i_file.is_open() && this->SrvData->getAutoindex() == "on")
	{
		this->est = " 200 Ok";
		this->cod = this->index;
	}
	else if (this->SrvData->getAutoindex() == "on")
	{
		std::getline(i_file, this->cod, '\0');
		i_file.close();
		this->cod = this->index;
		this->est = " 200 Ok";
	}
	else
	{
		i_file.close();
		this->est = " 204 No Content";
	}
}

void Response::get_index()
{
	if (!this->SrvData->getGet())										
		throw(ErrorResponseException("405 ERROR: Method Not Allowed"));
	std::string	archivo = this->root + this->SrvData->getIndex();
	std::ifstream i_file;
	i_file.open(archivo, std::ifstream::binary);
	if (!i_file.is_open() && this->SrvData->getAutoindex() == "off")
		throw(ErrorResponseException("403 Error: Forbidden"));
	else if (!i_file.is_open() && this->SrvData->getAutoindex() == "on")
	{
		this->cod = this->index;
		this->est = " 200 Ok";
	}
	else
	{
 		if (Response::cgi_option())
		{
			CGIexec	cgi(this->SrvData->getLocation(this->cgi_type), this->cgi_type, this->type_path, this->data_post);
			this->cod = cgi.getOutput();
			this->est = " 200 OK";
			return;
		}
		std::ostringstream o_file;
		o_file<<i_file.rdbuf();
		this->cod = o_file.str();
		if (this->cod.length() == 0)
		{
			i_file.close();
			this->est = " 204 Error: No content";
		}
		else
			this->est = " 200 Ok";
	}
}

void	Response::check_get_vrt_host(void)
{
	std::string	read;
	std::vector<CFGcluster>::iterator it;

	for (it = this->SrvData->cluster->begin(); it != this->SrvData->cluster->end(); it++)
	{
		if (it->getListen() == this->SrvData->getListen() && it->getVirtualServer())
		{
			if (!it->getGet())
				throw(ErrorResponseException("405 ERROR: Method Not Allowed"));
			if (access((it->getRoot() + this->data_resp["url"].substr((this->data_resp["url"].rfind('/') + 1))).c_str(), F_OK) == 0)
			{
				std::string	archivo = it->getRoot() + this->data_resp["url"].substr((this->data_resp["url"].rfind('/') + 1));
				std::ifstream i_file;
				i_file.open(archivo, std::ifstream::binary);
				if (!i_file.is_open())
					throw(ErrorResponseException("404 ERROR: Not Found"));
				else
				{
					if (Response::cgi_option())
					{
						CGIexec	cgi(this->SrvData->getLocation(this->cgi_type), this->cgi_type, this->type_path, this->data_post);
						this->cod = cgi.getOutput();
						this->est = " 200 OK";
						return;
					}
					std::cout<<"ARCHIVO VACIO LENGHT:"<<this->cod.length()<<"\n";
					std::ostringstream o_file;
					o_file<<i_file.rdbuf();
					this->cod = o_file.str();
					if (this->cod.length() == 0)
					{
						i_file.close();
						Response::eval_get_index(i_file);
					}
					else
						this->est = " 200 Ok";
					return;
				}		
			}			
		}
	}
	throw(ErrorResponseException("404 ERROR: Not Found"));
}

void	Response::get_content_type(void)
{
	if (this->data_resp["url"].rfind('.') != std::string::npos)
	{
		if (this->data_resp["url"].substr(this->data_resp["url"].rfind('.')) == ".html")
			this->file_type = "text/html";
		else if (this->data_resp["url"].substr(this->data_resp["url"].rfind('.')) == ".jpg")
			this->file_type = "image/jpeg";
		else if (this->data_resp["url"].substr(this->data_resp["url"].rfind('.')) == ".css")
			this->file_type = "text/css";
		else if (this->data_resp["url"].substr(this->data_resp["url"].rfind('.')) == ".pdf")
			this->file_type = "application/pdf";
		else if (this->data_resp["url"].substr(this->data_resp["url"].rfind('.')) == ".png")
			this->file_type = "image/png";		
		else
			this->file_type = "text/html";
	}
}

void Response::get_url()
{
	std::string	read;

	if (access((this->root + this->data_resp["url"]).c_str(), F_OK) == 0)
	{
		if (!this->SrvData->getGet())
			throw(ErrorResponseException("405 ERROR: Method Not Allowed"));
		std::string	archivo = this->root + this->data_resp["url"];
		std::ifstream i_file;
		i_file.open(archivo, std::ifstream::binary);
		if (!i_file.is_open())
			throw(ErrorResponseException("404 ERROR: Not Found"));
		else
		{
			if (Response::cgi_option())
			{
				if (this->v_post.size() > 0)
					Response::get_postdata();
				CGIexec	cgi(this->SrvData->getLocation(this->cgi_type), this->cgi_type, this->type_path, this->data_post);
				this->cod = cgi.getOutput();
				this->est = " 200 OK";
				return;
			}
			std::ostringstream o_file;
			o_file<<i_file.rdbuf();
			this->cod = o_file.str();
			if (this->cod.length() == 0)
			{
				i_file.close();
				Response::eval_get_index(i_file);
			}
			else
				this->est = " 200 Ok";
		}		
	}
	else
		throw(ErrorResponseException("404 Error: Not Found"));
}

std::string	Response::Rsp_get(void)
{
	try
	{
		Response::generate_index(this->root + this->data_resp["url"]);
		if (this->data_resp.find("Referer") != this->data_resp.end() || this->data_resp["url"] != "")
			Response::get_url();
		else if (this->data_resp["url"] == "")
			Response::get_index();
		else
			throw(ErrorResponseException("204 No Content"));
	}
	catch(const std::exception& e)
	{
		return (Response::Rsp_excep(e.what()));
	}
	return (Response::generate_response());
}

void	Response::clean_multipost(void)
{
	size_t pos;
	std::string	orig_code (this->data_resp["boundary"]);

	std::string code("--" + orig_code + "\r\n");
	
	pos = this->v_post.find(code);
	
	this->v_post.erase(pos, ((size_t)code.length()));
	this->v_post.erase(0, (this->v_post.find("\r\n\r\n")+4));
	
	code = "--" + orig_code + "--";
	pos = this->v_post.find(code);
	this->v_post.erase((pos - 2), (size_t)(this->v_post.length() - pos+2));

}


void		Response::get_postdata(void)
{
	size_t	pos = this->v_post.find('&');

	if (pos == std::string::npos)
		this->data_post[this->v_post.substr(0, this->v_post.find('='))] = this->v_post.substr((this->v_post.find('=') + 1));
	else
	{
		std::string	aux = this->v_post;
		pos = aux.find('&');
		while (pos != std::string::npos)
		{
			pos = aux.find('&');
			this->data_post[aux.substr(0, aux.find('='))] = aux.substr((aux.find('=') + 1), (pos - 1 - aux.find('=')));
			aux = aux.substr(pos + 1);
			pos = aux.find('&');
		}
		this->data_post[aux.substr(0, aux.find('='))] = aux.substr((aux.find('=') + 1), (pos - 1 - aux.find('=')));
	}
}


bool	Response::cgi_option(void)
{
	if (this->data_resp.find("Referer") != this->data_resp.end() || this->data_resp.find("url") != this->data_resp.end())
	{
		if (this->data_resp["Referer"].find(".php") != std::string::npos || this->data_resp["url"].find(".php") != std::string::npos)
		{
			this->cgi_type = "*.php";
			this->type_path = this->root + this->data_resp["url"].substr((this->data_resp["url"].rfind('/') + 1));
			return (true);
		}
		else if (this->data_resp["Referer"].find(".py") != std::string::npos || this->data_resp["url"].find(".py") != std::string::npos)
		{
			this->cgi_type = "*.py";
			this->type_path = this->root + this->data_resp["url"].substr((this->data_resp["url"].rfind('/') + 1));			
			return (true);
		}
	}
	if (this->SrvData->getIndex().find(".php") != std::string::npos && this->data_resp["url"].length() < 2)
	{
		this->cgi_type = "*.php";
		this->type_path = this->root + this->SrvData->getIndex();
		return (true);
	}
	else if (this->SrvData->getIndex().find(".py") != std::string::npos && this->data_resp["url"].length() < 2)
	{
		this->cgi_type = "*.py";
		this->type_path = this->root + this->SrvData->getIndex();
		return (true);		
	}
	return (false);
}

std::string	Response::Rsp_post(void)
{
	std::string	archivo;

	try
	{
		if (!this->SrvData->getPost())
			throw(ErrorResponseException("405 ERROR: Method Not Allowed"));
		if (Response::cgi_option())
		{
			Response::get_postdata();
			CGIexec	cgi(this->SrvData->getLocation(this->cgi_type), this->cgi_type, this->type_path, this->data_post);
			this->cod = cgi.getOutput();
			this->est = " 200 OK";
			return (Response::generate_response());
		}
		if (this->data_resp.find("boundary") != this->data_resp.end())
			Response::clean_multipost();
		if (this->data_resp.find("filename") != this->data_resp.end())
			archivo = this->dir_upload + this->data_resp["filename"];
		else
			archivo = this->dir_upload + this->data_resp["url"];
		std::ofstream file(archivo, std::ios::binary);
		file.write(this->v_post.c_str(), this->v_post.size());
		file.close();
		this->est = " 200 OK";
	}
	catch(const std::exception& e)
	{
		return (Response::Rsp_excep(e.what()));
	}
	return (Response::generate_response());
}


void	Response::check_del_vrt_host(void)
{
	std::string	read;
	std::vector<CFGcluster>::iterator it;

	for (it = this->SrvData->cluster->begin(); it != this->SrvData->cluster->end(); it++)
	{
		if (it->getListen() == this->SrvData->getListen() && it->getVirtualServer())
		{
			if (!it->getDel())
				throw(ErrorResponseException("405 ERROR: Method Not Allowed"));
			if (access((it->getRoot() + find_route(this->data_resp["url"])).c_str(), F_OK) == 0)
			{
				std::string	archivo = it->getRoot() + this->data_resp["url"];
				std::remove(archivo.c_str());
				this->est = " 200 OK";
				return;
			}		
		}			
	}
	throw(ErrorResponseException("404 ERROR: Not Found"));
}

std::string	Response::Rsp_delete(void)
{
	try
	{
		if (access((this->root + find_route(this->data_resp["url"])).c_str(), F_OK) == 0)
		{
			if (!this->SrvData->getDel())
				throw(ErrorResponseException("405 Error: Method Not Allowed"));
			std::string	archivo = this->root + this->data_resp["url"];
			std::remove(archivo.c_str());
			this->est = " 200 OK\n";
		}
		else if (this->SrvData->getDefaultServer()== true)
			Response::check_del_vrt_host();
		else
			throw(ErrorResponseException("404 ERROR: Not Found"));

	}
	catch(const std::exception& e)
	{
		return (Response::Rsp_excep(e.what()));
	}
	return (Response::generate_response());
}

std::string	Response::generate_response(void)
{
	Response::get_content_type();
	
	if (this->SrvData->getReturnState())
		return (this->data_resp["protoc"] + " " + nbrtostring(this->SrvData->getReturnNumber()) + 
				"\nLocation: " + this->SrvData->getReturnPage() + "\n\n");
	return (this->data_resp["protoc"] + this->est + 
				"\nDate: " + get_date() + "\n" +
				"Server: " + this->SrvData->getSrvNme() + "\n" +
				"Content-Type: " + this->file_type + "; charset=utf-8\n" + 
				"Content-Length: " + nbrtostring(this->cod.length()) + "\n" +
				"\n" + this->cod +"\n");
}

std::string	Response::Rsp_excep(std::string error)
{
	this->est = error;
	
	std::string	archivo = SrvData->getErrorPagesPath(atoi(this->est.substr(0, this->est.find(' ')).c_str()));
	std::ifstream i_file;
	i_file.open(archivo, std::ifstream::in);
	if (!i_file.is_open())
	{
		std::cerr<<"Error. No such file: "<<archivo<<" "<<this->est<<std::endl;
		this->cod = "Error. No such file: " + archivo + "\n";
	}
	else
		std::getline(i_file, this->cod, '\0');
	this->est = " " + this->est;
	return(Response::generate_response());
}

std::string	Response::check_type(void)
{
	if (this->data_resp["type"] == "GET")
		return (Response::Rsp_get());
	else if (this->data_resp["type"] == "POST")
		return (Response::Rsp_post());
	else if (this->data_resp["type"] == "DELETE")
		return (Response::Rsp_delete());
	else
		return (Response::Rsp_excep("405 Error: Method Not Allowed"));
}

std::string	Response::check_data_resp(void)
{
	try
	{
		if (this->data_resp.find("type") == this->data_resp.end())
			throw ErrorResponseException("405 Error: Method Not Allowed");
		if (this->data_resp.at("type") != "GET" && this->data_resp.at("type") != "POST" && this->data_resp.at("type") != "DELETE")
			throw ErrorResponseException("405 Error: Method Not Allowed");
		if (this->data_resp["protoc"] != "HTTP/1.1")
			throw ErrorResponseException("505 Error: HTTP Version Not Supported");
		if (this->data_resp.at("type") == "POST" && this->data_resp.find("Content-Length") == this->data_resp.end())
			throw ErrorResponseException("411 Error: Length Required");
		if (this->data_resp.at("type") == "POST" && (size_t)std::atol(this->data_resp.at("Content-Length").c_str()) != this->v_post.length() && this->data_resp.find("multipart") != this->data_resp.end())
			throw ErrorResponseException("400 Error: Bad Request");
 		if ((size_t)this->SrvData->getBodyBuffer() < (size_t)this->v_post.size())
			throw ErrorResponseException("413 Error: Payload Too Large");
	}
	catch(const std::exception& e)
	{
		return (Response::Rsp_excep(e.what()));
	}
	return (Response::check_type());
}

bool	is_dir(std::string path)
{
	struct stat data;
	
	if (stat(path.c_str(), &data) != 0)
		return (false);
	return (S_ISDIR(data.st_mode));
	
}

std::string	Response::get_data_resp(void)
{
	try
	{
		std::string	aux;
		std::string line(this->request);

		aux = this->request.substr(0, this->request.find('\n') - 1);
		this->data_resp["type"] = aux.substr(0, aux.find(' '));
		this->data_resp["url"] = aux.substr(aux.find(' ') + 1, aux.rfind(' ') - aux.find(' ') - 1);
		if (this->data_resp["url"].rfind('/') == this->data_resp["url"].length() - 1)
			this->data_resp["url"] = this->data_resp["url"].erase(this->data_resp["url"].rfind('/'), 1);
		
		if (this->data_resp["url"].rfind('?') != std::string::npos)
		{
			this->v_post = this->data_resp["url"].substr((this->data_resp["url"].rfind('?') + 1));
			this->data_resp["url"] = this->data_resp["url"].substr(0, this->data_resp["url"].rfind('?'));
		} 

		this->data_resp["protoc"] = aux.substr(aux.rfind(" ") + 1);
		std::istringstream input(this->request);
		for (; std::getline(input, aux, '\n'); )
		{
			if (aux.find('\r') != std::string::npos)
				aux = aux.erase(aux.find('\r'), 1);
			Response::get_data_value(aux);
		}
		size_t pos  = line.find("\r\n\r\n");
		if (pos != std::string::npos && this->v_post.size() == 0)
			this->v_post = line.substr(pos + 4);
		Response::define_virtualHost();
		if (this->SrvData->getReturnState())
			return(Response::generate_response());
		if (is_dir(this->root + this->data_resp["url"]) && this->data_resp["url"].size() != 0
			&& this->SrvData->getAutoindex() == "off")
		{
			std::ifstream i_file;
			i_file.open(this->root + this->SrvData->getIndex(), std::ifstream::binary);
			if (!i_file.is_open())
				throw(ErrorResponseException("404 ERROR: Not Found"));
			else
			{
				std::ostringstream o_file;
				o_file<<i_file.rdbuf();
				this->cod = o_file.str();
				this->est = " 200 Ok";
			}
			return (Response::generate_response());
		}
		return (Response::check_data_resp());
	}
	catch(const std::exception& e)
	{
		return (Response::Rsp_excep(e.what()));
	}
}

void	Response::define_virtualHost(void)
{
	if (this->SrvData->getDefaultServer())
	{
		if (this->data_resp["Host"].find(this->SrvData->getSrvNme()) == std::string::npos)
		{
			std::vector<CFGcluster>::iterator it;
			for (it = this->SrvData->cluster->begin(); it != this->SrvData->cluster->end(); it++)
			{
				if (it->getListen() == this->SrvData->getListen() && it->getVirtualServer() == 1
					&& this->data_resp["Host"].find(it->getSrvNme()) != std::string::npos)
				{
					if ((this->data_resp["type"] == "GET" || this->data_resp["type"] == "DELETE")
						&&((it->getRoot() + this->data_resp["url"].substr((this->data_resp["url"].rfind('/') + 1))).c_str(), F_OK) == 0)
						this->root = it->getRoot();
					else if (this->data_resp["type"] == "POST")
						this->root = it->getRoot();
					this->SrvData = &(*it);
					this->index = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n	<meta charset=\"UTF-8\">\n	<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n		<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n		<title>ElYogurServer</title>\n</head>\n<body>\n	<h1 style=\"text-align:center\">Hello From Index " + this->SrvData->getSrvNme() + "</h1>\n";
				}
			}
		}
	}
}