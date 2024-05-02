/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CFGlocation.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaromero <jaromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 00:12:20 by jaromero          #+#    #+#             */
/*   Updated: 2023/12/05 13:08:24 by jaromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CFGlocation.hpp"

CFGlocation::CFGlocation(void)
{
    this->name_location.clear();
    this->root.clear();
    this->index.clear();
    this->try_files.clear();
    this->allow_methods.clear();
    this->autoindex.clear();
    this->fastcgi_pass.clear();
    this->fastcgi_index.clear();
    this->nbrparam = 0;
    this->fastcgi_param.clear();
    this->client_max_body_size = 0;
    this->upload_store.clear();
    this->upload_pass.clear();
    this->nbr_upload_set = 0;
    this->upload_set_form_field.clear();
}

CFGlocation::CFGlocation(CFGlocation const &copy)
{
    this->name_location = copy.name_location;
    this->root = copy.root;
    this->index = copy.index;
    this->try_files = copy.try_files;
    this->allow_methods = copy.allow_methods;
    this->autoindex = copy.autoindex;
    this->fastcgi_pass = copy.fastcgi_pass;
    this->fastcgi_index = copy.fastcgi_index;
    this->nbrparam = copy.nbrparam;
    this->fastcgi_param = copy.fastcgi_param;
    this->client_max_body_size = copy.client_max_body_size;
    this->upload_store = copy.upload_store;
    this->upload_pass = copy.upload_pass;
    this->nbr_upload_set = copy.nbr_upload_set;
    this->upload_set_form_field = copy.upload_set_form_field;
}

CFGlocation::~CFGlocation()
{
    this->clear();
}

CFGlocation&    CFGlocation::operator=(CFGlocation const &pointer)
{
    this->name_location = pointer.name_location;
    this->root = pointer.root;
    this->index = pointer.index;
    this->try_files = pointer.try_files;
    this->allow_methods = pointer.allow_methods;
    this->autoindex = pointer.autoindex;
    this->fastcgi_pass = pointer.fastcgi_pass;
    this->fastcgi_index = pointer.fastcgi_index;
    this->nbrparam = pointer.nbrparam;
    this->fastcgi_param = pointer.fastcgi_param;
    this->client_max_body_size = pointer.client_max_body_size;
    this->upload_store = pointer.upload_store;
    this->upload_pass = pointer.upload_pass;
    this->nbr_upload_set = pointer.nbr_upload_set;
    this->upload_set_form_field = pointer.upload_set_form_field;

    return (*this);
}

void    CFGlocation::clear(void)
{
    this->name_location.clear();
    this->root.clear();
    this->index.clear();
    this->try_files.clear();
    this->allow_methods.clear();
    this->autoindex.clear();
    this->fastcgi_pass.clear();
    this->fastcgi_index.clear();
    this->nbrparam = 0;
    this->fastcgi_param.clear();
    this->client_max_body_size = 0;
    this->upload_store.clear();
    this->upload_pass.clear();
    this->nbr_upload_set = 0;
    this->upload_set_form_field.clear();
}

void    CFGlocation::printData(void)
{
    std::cout << "Name location: " << name_location << std::endl;
    std::cout << "root: " << root << std::endl;
    std::cout << "index: " << index << std::endl;
    std::cout << "try_files: " << try_files << std::endl;
    std::cout << "allow_methods: " << allow_methods << std::endl;
    std::cout << "autoindex: " << autoindex << std::endl << std::endl;

    std::cout << "fastcgi_pass: " << fastcgi_pass << std::endl;
    std::cout << "fastcgi_index: " << fastcgi_index << std::endl;
    std::cout << "Number of fastcgi_param: " << nbrparam << std::endl;
    for (int i = 0; i < getNbrparam(); i++)
        std::cout << "fastcgi_param: " << fastcgi_param[i] << std::endl << std::endl;
    
    std::cout << "client_max_body_size: " << client_max_body_size << std::endl;
    std::cout << "upload_store: " << upload_store << std::endl;
    std::cout << "upload_pass: " << upload_pass << std::endl;
    std::cout << "Number of upload_set_form_field: " << nbr_upload_set << std::endl;
    for (int i = 0; i < getNbrUploadSet(); i++)
        std::cout << "upload_set_form_field: " << upload_set_form_field[i] << std::endl;
}

/*------- SETS ---------*/

void        CFGlocation::SetName(std::string data)
{
    this->name_location = data;
}

void        CFGlocation::SetRoot(std::string data)
{
    this->root = data;
}

void        CFGlocation::SetIndex(std::string data)
{
    this->index = data;
}

void        CFGlocation::SetTryFiles(std::string data)
{
    this->try_files = data;
}

void        CFGlocation::SetMethods(std::string data)
{
    this->allow_methods = data;
}

void        CFGlocation::SetAutoindex(std::string data)
{
    this->autoindex = data;
}

/*---------------------------------*/

void        CFGlocation::SetCGIpass(std::string data)
{
    this->fastcgi_pass = data;
}

void        CFGlocation::SetCGIindex(std::string data)
{
    this->fastcgi_index = data;
}

void        CFGlocation::SetNbrparam(int data)
{
    this->nbrparam = data;
}

void        CFGlocation::SetNewCGIparam(std::string data)
{
    this->fastcgi_param.push_back(data);
}

/*---------------------------------*/

void        CFGlocation::SetMaxBodySize(std::string data)
{
    unsigned int    len;

    len = 0;
    if (data == "")
        this->client_max_body_size = 5000;
    else
    {
        if (data.find("M", 0) != std::string::npos)
            len = 1048576;
        else if (data.find("K", 0) != std::string::npos)
            len = 1024;
        if (len == 0)
            this->client_max_body_size = std::atoi(data.c_str());
        else    
            this->client_max_body_size = std::atoi(data.c_str()) * len;
    }
}

void        CFGlocation::SetUploadStore(std::string data)
{
    this->upload_store = data;
}

void        CFGlocation::SetUploadPass(std::string data)
{
    this->upload_pass = data;
}

void        CFGlocation::SetNbrUploadSet(int data)
{
    this->nbr_upload_set = data;
}

void        CFGlocation::SetUploadFormField(std::string data)
{
    this->upload_set_form_field.push_back(data.c_str());
}

/*------- GETS ---------*/

std::string CFGlocation::getName(void)
{
    return (this->name_location);
}

std::string CFGlocation::getRoot(void)
{
    return (this->root);
}

std::string CFGlocation::getIndex(void)
{
    return (this->index);
}

std::string CFGlocation::getTryFiles(void)
{
    return (this->try_files);
}

std::string CFGlocation::getMethods(void)
{
    return (this->allow_methods);
}

std::string CFGlocation::getAutoindex(void)
{
    return (this->autoindex);
}

/*-------------------*/

std::string CFGlocation::getCGIpass(void)
{
    return (this->fastcgi_pass);
}

std::string CFGlocation::getCGIindex(void)
{
    return (this->fastcgi_index);
}

int         CFGlocation::getNbrparam(void)
{
    return (this->nbrparam);
}

std::string CFGlocation::getCGIparam(int nbr)
{
    return (this->fastcgi_param[nbr]);
}

/*-------------------*/

unsigned int    CFGlocation::getMaxBodySize(void)
{
    return (this->client_max_body_size);
}

std::string CFGlocation::getUploadStore(void)
{
    return (this->upload_store);
}

std::string CFGlocation::getUploadPass(void)
{
    return (this->upload_pass);
}

int         CFGlocation::getNbrUploadSet(void)
{
    return(this->nbr_upload_set);
}

std::string CFGlocation::getUploadFormField(int nbr)
{
    return (this->upload_set_form_field[nbr]);
}
