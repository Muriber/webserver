/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CFGcluster.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaromero <jaromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 23:13:44 by jaromero          #+#    #+#             */
/*   Updated: 2023/12/10 11:59:58 by jaromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CFGcluster.hpp"

CFGcluster::CFGcluster(void)
{
    this->srvName.clear();
    this->root.clear();
    this->index.clear();
    this->autoindex.clear();
    this->allow_methods.clear();
    this->retrn_page.clear();
    this->errorPages.clear();
    this->nbrErrorPages = 0;
    this->client_max_body_size = 0;
    this->listen = 0;
    this->get = false;
    this->post = false;
    this->del = false;
    this->def_server = false;
    this->virtual_server = false;
    this->retrn_page.clear();
    this->retrn_number = 0;
    this->retrn_state = false;
    this->locations.clear();
    this->cluster = NULL;
}

CFGcluster::CFGcluster(CFGcluster const &copy)
{
    this->srvName = copy.srvName;
    this->root = copy.root;
    this->index = copy.index;
    this->autoindex = copy.autoindex;
    this->allow_methods = copy.allow_methods;
    this->retrn_page = copy.retrn_page;
    this->errorPages = copy.errorPages;
    this->nbrErrorPages = copy.nbrErrorPages;
    this->client_max_body_size = copy.client_max_body_size;
    this->listen = copy.listen;
    this->get = copy.get;
    this->post = copy.post;
    this->del = copy.del;
    this->def_server = copy.def_server;
    this->virtual_server = copy.virtual_server;
    this->retrn_page = copy.retrn_page;
    this->retrn_number = copy.retrn_number;
    this->retrn_state = copy.retrn_state;
    this->locations = copy.locations;
    this->nbr_location = copy.nbr_location;
    this->cluster = copy.cluster;
}

CFGcluster::~CFGcluster()
{
}

CFGcluster& CFGcluster::operator=(CFGcluster &pointer)
{
    this->srvName = pointer.getSrvNme();
    this->root = pointer.getRoot();
    this->index = pointer.getIndex();
    this->autoindex = pointer.getAutoindex();
    this->allow_methods = pointer.getAllowmethods();
    this->listen = pointer.getListen();
    this->allow_methods = pointer.getAllowmethods();
    this->get = pointer.getGet();
    this->post = pointer.getPost();
    this->del = pointer.getDel();
    this->client_max_body_size = pointer.getBodyBuffer();
    this->nbr_location = pointer.getNbrLocation();
    for(int i = 0; i < pointer.getNbrLocation(); i++)
        this->locations.push_back(pointer.getLocations(i));
    this->errorPages = pointer.errorPages;
    this->cluster = pointer.cluster;
    this->retrn_number = pointer.retrn_number;
    this->retrn_page = pointer.retrn_page;
    this->retrn_state = pointer.retrn_state;
    this->cluster = pointer.cluster;
    
    return (*this);
}

void    CFGcluster::printLocation(void)
{
    int i;

    i = 0;
    while (i < this->nbr_location)
    {
        std::cout << "--- location " << i + 1 << " ---" << std::endl;
        this->locations[i].printData();
        i++;
    }
}

/*---------- SETS -------------*/

void    CFGcluster::SetSrvNme(std::string _name)
{
    this->srvName = _name;
}

void    CFGcluster::SetRoot(std::string _root)
{
    if (_root == "")
        this->root = "srcs/";
    else
        this->root = _root;
}

void    CFGcluster::SetIndex(std::string _index)
{
    if (_index == "")
        this->index = "index.html";
    else
        this->index = _index;
}

void    CFGcluster::SetAutoindex(std::string _autoindex)
{
    if (_autoindex == "")
        this->autoindex = "off";
    else
        this->autoindex = _autoindex;
}

void    CFGcluster::SetAllowmethods(std::string _allowmethods)
{
    if (_allowmethods == "")
        this->allow_methods = "GET, POST, DELETE";
    else
        this->allow_methods = _allowmethods;
}

void    CFGcluster::SetErrorPages(std::string data)
{
    std::string path;
    int         i;
    int         x;
    int         error;

    if (data == "")
        return;
    i = 0;
    while (data[i] == ' ' || data[i] == '\t')
        i++;
    x = i;
    while(data[x] >= '0' && data[x] <= '9')
        x++;
    error = atoi(data.substr(i, x).c_str());
    if (data[x] == ' ')
    {
        x++;
        path = data.substr(x, (data.length() - x));
        this->errorPages.insert(std::make_pair(error, path));
    }
}

void    CFGcluster::SetNbrErrorPages(int data)
{
    this->nbrErrorPages = data;
}

void    CFGcluster::SetListen(int _listen)
{
    this->listen = _listen;
}

void    CFGcluster::SetDefaultServer(void)
{
    this->def_server = true;
}

void    CFGcluster::SetVirtualServer(void)
{
    this->virtual_server = true;
}

void    CFGcluster::SetGet(std::string data)
{
    if (data == "GET")
		this->get = true;
}

void    CFGcluster::SetPost(std::string data)
{
    if (data == "POST")
		this->post = true;
}

void    CFGcluster::SetDel(std::string data)
{
    if (data == "DELETE")
		this->del = true;
}

void    CFGcluster::SetBodyBuffer(std::string data)
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

void    CFGcluster::SetNbrLocation(int  _nbr)
{
    this->nbr_location = _nbr;
}

void    CFGcluster::SetNewLocation(CFGlocation _data)
{
    this->locations.push_back(_data);
}

void    CFGcluster::SetReturnPage(std::string _data)
{
    int i;

    if (_data != "")
    {
        this->retrn_number = atoi(_data.c_str());
        if (this->retrn_number < 301 || (this->retrn_number > 303 && this->retrn_number != 307 && this->retrn_number != 308))
            return ;
        i = 0;
        while ((_data[i] >= '0' && _data[i] <= '9') || _data[i] == ' ')
            i++;
        if (_data[i] == '\n' || _data[i] == '\0')
            return ;
        this->retrn_page = _data.substr(i, (_data.length() - i));
        this->retrn_state = true;              
    }
}

/*-----------GETS-----------------*/

std::string     CFGcluster::getSrvNme(void)
{
    return (this->srvName);
}

std::string     CFGcluster::getRoot(void)
{
    return (this->root);
}

std::string     CFGcluster::getIndex(void)
{
    return (this->index);
}

std::string     CFGcluster::getAutoindex(void)
{
    return (this->autoindex);
}

std::string     CFGcluster::getAllowmethods(void)
{
    return (this->allow_methods);
}

std::string     CFGcluster::getErrorPagesPath(int nbr)
{
    std::map<int, std::string>::iterator it;
    std::string defPath = "httpcod/" + nbrtostring(nbr) + ".html";

    it = errorPages.begin();
    while (it != errorPages.end())
    {
        if (it->first == nbr)
            return (it->second);
        it++;
    }
    return (defPath);
}

std::string     CFGcluster::getReturnPage(void)
{
    return (this->retrn_page);
}

int     CFGcluster::getReturnNumber(void)
{
    return (this->retrn_number);
}

int     CFGcluster::getNbrErrorPages(void)
{
    return (this->nbrErrorPages);
}

int     CFGcluster::getListen(void)
{
    return (this->listen);
}

bool    CFGcluster::getReturnState(void)
{
    return (this->retrn_state);
}

bool    CFGcluster::getDefaultServer(void)
{
    return (this->def_server);
}

bool    CFGcluster::getVirtualServer(void)
{
    return (this->virtual_server);
}

bool    CFGcluster::getGet(void)
{
    return (this->get);
}

bool    CFGcluster::getPost(void)
{
    return (this->post);
}

bool    CFGcluster::getDel(void)
{
    return (this->del);
}

unsigned int     CFGcluster::getBodyBuffer(void)
{
    return (this->client_max_body_size);
}

int     CFGcluster::getNbrLocation(void)
{
    return (this->nbr_location);
}

CFGlocation     CFGcluster::getLocations(int i)
{
    return (this->locations[i]);
}

CFGlocation*    CFGcluster::getLocation(std::string _name)
{
    std::vector<CFGlocation>::iterator it;
    int                                 i;

    i = 0;
    it = locations.begin();
    while (it != locations.end())
    {
        if (it->getName().find(_name) != std::string::npos)
            return (&locations[i]);
        it++;
        i++;
    }
    return (NULL);
}

std::string     nbrtostring(size_t nbr)
{
    std::ostringstream stream;

    stream <<nbr;
    return(stream.str());
}