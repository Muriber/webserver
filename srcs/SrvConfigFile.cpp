/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SrvConfigFile.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaromero <jaromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 23:48:08 by jaromero          #+#    #+#             */
/*   Updated: 2023/12/10 11:44:44 by jaromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SrvConfigFile.hpp"

SrvConfigFile::SrvConfigFile(void)
{
    if (OpenFile("config/default.conf") != 0)
    {
        std::cout << "Fatal error!! Cant load config file!\n";
        exit (1);
    }
    if (CheckEmptyFile() == false)
    {
        std::cout << "Empty config file!\n";
        exit (1);
    }
    this->nbrSrv = nbrOccurrence(this->file, "server ");
    this->nbrSave = 0;
    while (this->nbrSave < this->nbrSrv)
    {
        if (SaveBlock(this->file, this->server, "server ", 0) != 0)
            return ;
        this->SrvCfg.push_back(SaveDataServer());
        this->nbrSave++;
        this->checkRead(this->server);
        server.clear();
    }
    this->SetDefaultServer();
    this->checkPorts();
    this->checkRead(this->file);
}

SrvConfigFile::SrvConfigFile(std::string infile)
{
    if (infile.find(".conf\0") == std::string::npos)
    {
        std::cout << "Invalid extension!\n";
        exit (1);
    }
    if (OpenFile(infile) != 0)
    {
        std::cout << "Fatal error!! Cant load config file!\n";
        exit (1);
    }
    if (CheckEmptyFile() == false)
    {
        std::cout << "Empty config file!\n";
        exit (1);
    }
    this->nbrSrv = nbrOccurrence(this->file, "server ");
    this->nbrSave = 0;
    while (this->nbrSave < this->nbrSrv)
    {
        if (SaveBlock(this->file, this->server, "server ", 0) != 0)
            return ;
        this->SrvCfg.push_back(SaveDataServer());
        this->nbrSave++;
        this->checkRead(this->server);
        server.clear();
    }
    this->SetDefaultServer();
    this->checkPorts();
    this->checkRead(this->file);
}

SrvConfigFile::SrvConfigFile(SrvConfigFile const &copy)
{
    this->file = copy.file;
    this->server = copy.server;
    this->location = copy.location;
    this->nbrSrv = copy.nbrSrv;
    this->nbrSave = copy.nbrSave;
    this->SrvCfg = copy.SrvCfg;
}

SrvConfigFile::~SrvConfigFile(void)
{
    infile.close();
    this->SrvCfg.clear();
}

bool    SrvConfigFile::CheckEmptyFile(void)
{
    int i;

    i = 0;
    while (file[i] != '\0')
    {
        if (file[i] > ' ' && file[i] < '~')
            return (true);
        i++;
    }
    return (false);
}

SrvConfigFile&   SrvConfigFile::operator=(SrvConfigFile &pointer)
{
    this->file = pointer.file;
    this->server = pointer.server;
    this->location = pointer.location;
    this->nbrSrv = pointer.nbrSrv;
    this->nbrSave = pointer.nbrSave;
    this->SrvCfg.clear();
    this->SrvCfg = pointer.SrvCfg;
    
    return (*this);
}

int    SrvConfigFile::OpenFile(std::string _infile)
{
    infile.open(_infile);
    if (infile.fail())
    {
        std::cout << "Failed open file " << _infile << std::endl;
        return (1);
    }
    if (SaveFile() != 0)
    {
        std::cout << "In file: " << _infile << std::endl;
        return (2);
    }
    return (0);
}

int    SrvConfigFile::SaveFile(void)
{
    std::string str;
    int count;

    count = 0;
    while (!infile.eof())
    {
        getline(infile, str);
        if (str[0] != '#')
        {        
            if (count)
                this->file = file + "\n" + str;
            else
                this->file = file + str;
            count++;
        }
    }
    if (CheckKey() != 0)
    {
        std::cout << "Fatal error! Key`s are open in config file! \n";
        exit (1);
    }
    return (0);
}

int     SrvConfigFile::CheckKey(void)
{
    int countIN;
    int countOUT;
    int i;

    i = 0;
    countIN = 0;
    while (file.find("{", i) != std::string::npos)
    {
        i = file.find("{", i);
        countIN++;
        i++;
    }
    i = 0;
    countOUT = 0;
    while (file.find("}", i) != std::string::npos)
    {
        i = file.find("}", i);
        countOUT++;
        i++;
    }
    return (countIN - countOUT);
}

void    SrvConfigFile::checkRead(std::string data)
{
    int     i;

    i = 0;
    while (data[i] != '\0')
    {
        if (data[i] != ' ' && data[i] != '\n' && data[i] != '\t')
        {
            std::cout << "Error config file! invalid data..." << std::endl;
            exit (1);
        }
        i++;
    }
}

int    SrvConfigFile::SaveBlock(std::string &file, std::string &dest, std::string search, int del)
{
    int i;
    int init;
    int end;
    int e;

    i = 0;
    init = 1;
    end = 0;
    if (file.find(search, i) != std::string::npos)
    {
        i = file.find(search, i);
        file.erase(i, (search.length() + del));
        i = file.find("{", i);
        i++;
        e = i;
        while(init != end)
        {
            if (file[e] == '{')
                init++;
            if (file[e] == '}')
                end++;
            e++;
        }
        dest = file.substr((i + 1), ((e - 2) - i));
        file.erase((i - 1), ((e + 1) - i));
        return (0);
    }
    return (1);
}

void        SrvConfigFile::ConfigMethod(std::string data, CFGcluster &cfg)
{
    if (data.find("GET") != std::string::npos)
        cfg.SetGet("GET");
    if (data.find("POST") != std::string::npos)
        cfg.SetPost("POST");
    if (data.find("DELETE") != std::string::npos)
        cfg.SetDel("DELETE");
}

std::string     SrvConfigFile::saveLocationName(void)
{
    int         i;
    int         e;

    i = server.find("location ") + 9;
    e = i;
    while (server[e] != '\n' && server[e] != '{' && server[e] != '\0')
        e++;
    if (server[e] == '\0')
    {
        std::cout << "Fatal error! Cant parse file1!\n";
        exit(1);
    }
    return (server.substr(i, (e - i)));
}

void          SrvConfigFile::SaveDataLocation(CFGcluster &aux, CFGlocation &aux1)
{
    aux.SetNbrLocation(this->nbrOccurrence(this->server, "location "));
    while(server.find("location ") != std::string::npos)
    {
        aux1.SetName(this->saveLocationName());
        this->SaveBlock(this->server, this->location, "location ", aux1.getName().length());
        aux1.SetRoot(ExtractData("root ", location));
        aux1.SetIndex(ExtractData("index ", location));
        aux1.SetTryFiles(ExtractData("try_files ", location));
        aux1.SetMethods(ExtractData("allow_methods ", location));
        aux1.SetAutoindex(ExtractData("autoindex ", location));
        aux1.SetCGIpass(ExtractData("fastcgi_pass ", location));
        aux1.SetCGIindex(ExtractData("fastcgi_index ", location));
        aux1.SetNbrparam(this->nbrOccurrence(this->location, "fastcgi_param "));
        for(int i = 0; i < aux1.getNbrparam(); i++)
            aux1.SetNewCGIparam(ExtractData("fastcgi_param ", location));
        aux1.SetMaxBodySize(ExtractData("client_max_body_size ", location));
        aux1.SetUploadStore(ExtractData("upload_store ", location));
        aux1.SetUploadPass(ExtractData("upload_pass ", location));
        aux1.SetNbrUploadSet(this->nbrOccurrence(this->location, "upload_set_form_field "));
        for(int i = 0; i < aux1.getNbrUploadSet(); i++)
            aux1.SetUploadFormField(ExtractData("upload_set_form_field ", location));
        aux.SetNewLocation(aux1);
        aux1.clear();
        this->checkRead(this->location);
        this->location.clear();
    }
}

CFGcluster    SrvConfigFile::SaveDataServer(void)
{
    CFGcluster  aux;
    CFGlocation aux1;
    
    this->SaveDataLocation(aux, aux1);
    aux.SetRoot(ExtractData("root ", server));
    aux.SetAutoindex(ExtractData("autoindex ", server));
    aux.SetSrvNme(ExtractData("server_name ", server));
    aux.SetIndex(ExtractData("index ", server));
    aux.SetAllowmethods(ExtractData("allow_methods ", server));
    aux.SetNbrErrorPages(this->nbrOccurrence(server, "error_page "));
    for (int i = 0; i < aux.getNbrErrorPages(); i++)
        aux.SetErrorPages(ExtractData("error_page ", server));
    this->ConfigMethod(aux.getAllowmethods(), aux);
    aux.SetBodyBuffer(ExtractData("client_body_buffer_size ", server));
    aux.SetReturnPage(ExtractData("return ", server));
    this->SaveListen(&aux);
    return (aux);
}

void    SrvConfigFile::checkListen(CFGcluster *aux)
{
    if (aux->getListen() > 65535 || aux->getListen() < 0 
        || (aux->getListen() <= 1024 && aux->getListen() != 80 && aux->getListen() != 443))
    {
        if (aux->getListen() < 0)
            std::cout << "Error! cant create negative port!!\n";
        else if (aux->getListen() > 65535)
            std::cout << "Error! port is out of range!\n";
        else
            std::cout << "Error! listen port reserved for system!\n";
        exit (1);
    }
}

void    SrvConfigFile::SaveListen(CFGcluster *aux)
{
    std::string listen;
    int         nbr;

    aux->cluster = &this->SrvCfg;
    listen = ExtractData("listen ", server);
    if (listen == "")
    {
        std::cout << "No port mapping!\n";
        exit (1);
    }
    aux->SetListen(std::atoi(listen.c_str()));
    this->checkListen(aux);
    nbr = this->nbrOccurrence(server, "listen ");
    for (int i = 0; i < nbr; i++)
    {
        this->nbrSave++;
        this->nbrSrv++;
        CFGcluster aux1;
        aux1 = *aux;
        listen = ExtractData("listen ", server);
        aux1.SetListen(std::atoi(listen.c_str()));
        this->checkListen(&aux1);
        this->SrvCfg.push_back(aux1);
    }
}

std::string SrvConfigFile::ExtractData(std::string search, std::string &origin)
{
    std::string substr;
    int i;
    int e;

    i = 0;
    e = 0;
    if (origin.find(search, 0) != std::string::npos)
    {
        i = origin.find(search, 0);
        if (origin[i - 1] != ' ' && origin[i - 1] != '\t' && origin[i - 1] != '\n')
            return ("");
        i = i + search.length();
        e = i;
        while (origin[e] != ';' && origin[e] != '\n' && origin[e] != '\0')
            e++;
        if (origin[e] == '\n' || origin[e] == '\0')
        {
            std::cout << "Fatal error, cant parse file config!\n";
            exit(1);
        }
        substr = origin.substr(i, e - i); 
        this->DeleteDataServer(search, origin);
        return (substr);
    }
    return ("");
}

void SrvConfigFile::DeleteDataServer(std::string search, std::string &origin)
{
    int i;
    int e;

    i = 0;
    e = 0;
    if (origin.find(search, 0) != std::string::npos)
    {
        i = origin.find(search, 0);
        if (origin[i - 1] != ' ' && origin[i - 1] != '\t' && origin[i - 1] != '\n')
            return;
        e = i;
        while (origin[e] != ';' && origin[e] != '\n')
            e++;
        origin.erase(i, ((e + 1) - i));
    }
}

int     SrvConfigFile::nbrOccurrence(std::string &file, std::string search)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (file.find(search, i) != std::string::npos)
    {
        i = file.find(search, i);
        count++;
        i++;
    }
    return (count);
}

void    SrvConfigFile::checkPorts(void)
{
    int i;
    int x;
    int nbr;
    int def;

    nbr = 0;
    def = 0;
    x = 0;
    i = 0;
    while (i < this->nbrSrv)
    {
        while (x < this->nbrSrv)
        {
            if (x != i)
            {
                if (this->SrvCfg[i].getListen() == this->SrvCfg[x].getListen())
                {
                    if (SrvCfg[i].getSrvNme() == SrvCfg[x].getSrvNme())
                    {
                        std::cout << "Fatal error!! Cant create server with same port and same config!!\n";
                        exit (1);
                    }
                    nbr++;
                    if (SrvCfg[i].getDefaultServer() == true)
                        def++;
                    if (SrvCfg[x].getDefaultServer() == true)
                        def++;
                }
            }
            x++;
        }
        if (nbr != 0 && def != 1)
        {
            std::cout << "Fatal error!! Cant create server with same port!!";
            exit (1);
        }
        nbr = 0;
        def = 0;
        i++;
        x = 0;
    }
}

void    SrvConfigFile::SetDefaultServer()
{
    std::vector<CFGcluster>::iterator   init;
    std::vector<CFGcluster>::iterator   end;

    init = this->SrvCfg.begin();
    end = this->SrvCfg.begin();
    while (init != this->SrvCfg.end())
    {
        while (end != this->SrvCfg.end())
        {
            if (init != end)
            {
                if (init->getListen() == end->getListen())
                {
                    if (init->getDefaultServer() == false && init->getVirtualServer() == false)
                        init->SetDefaultServer();
                    end->SetVirtualServer();
                }
            }
            end++;
        }
        init++;
        end = init;
    }
}


void    SrvConfigFile::printDataCluster(void)
{
    int i;

    i = 0;
    while (i < this->nbrSrv)
    {
        std::cout << "----- Server " << i + 1 << ": -----\n\n"
            << "Server Name: " << this->SrvCfg[i].getSrvNme() << std::endl
            << "Root: " << this->SrvCfg[i].getRoot() << std::endl
            << "Index: " << this->SrvCfg[i].getIndex() << std::endl
            << "Autoindex: " << this->SrvCfg[i].getAutoindex() << std::endl
            << "Allow_Methods: " << this->SrvCfg[i].getAllowmethods() << std::endl
            << "GET: " << this->SrvCfg[i].getGet() << std::endl
            << "POST: " << this->SrvCfg[i].getPost() << std::endl
            << "DEL: " << this->SrvCfg[i].getDel() << std::endl
            << "client_body_buffer_size: " << this->SrvCfg[i].getBodyBuffer() << std::endl
            << "error_page 204: " << this->SrvCfg[i].getErrorPagesPath(204) << std::endl
            << "error_page 400: " << this->SrvCfg[i].getErrorPagesPath(400) << std::endl
            << "error_page 404: " << this->SrvCfg[i].getErrorPagesPath(404) << std::endl
            << "error_page 405: " << this->SrvCfg[i].getErrorPagesPath(405) << std::endl
            << "error_page 411: " << this->SrvCfg[i].getErrorPagesPath(411) << std::endl
            << "error_page 505: " << this->SrvCfg[i].getErrorPagesPath(505) << std::endl
            << "return state:   " << this->SrvCfg[i].getReturnState() << std::endl
            << "return number:  " << this->SrvCfg[i].getReturnNumber() << std::endl
            << "return page:    " << this->SrvCfg[i].getReturnPage() << std::endl
            << "Default server: " << this->SrvCfg[i].getDefaultServer() << std::endl
            << "Virtual server: " << this->SrvCfg[i].getVirtualServer() << std::endl
            << "listen: " << this->SrvCfg[i].getListen() << std::endl;
        this->SrvCfg[i].printLocation();
        std::cout << std::endl;
        i++;
    }
}

int SrvConfigFile::getnbrSrv(void)
{
    return (this->nbrSrv);
}

