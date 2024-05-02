/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SrvConfigFile.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaromero <jaromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 23:47:05 by jaromero          #+#    #+#             */
/*   Updated: 2023/12/10 11:29:28 by jaromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SRVCONFIGFILE_HPP
#define SRVCONFIGFILE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "CFGcluster.hpp"
#include "CFGlocation.hpp"

class SrvConfigFile
{
    private:
        std::ifstream       infile;
        std::string         file;
        std::string         server;
        std::string         location;
        int                 nbrSrv;
        int                 nbrSave;

        int             OpenFile(std::string _infile);
        int             SaveFile(void);
        int             CheckKey(void);
        int             SaveBlock(std::string &file, std::string &dest, std::string search, int del);
        CFGcluster      SaveDataServer(void);
        std::string     ExtractData(std::string search, std::string &origin);
        void            SaveDataLocation(CFGcluster &aux, CFGlocation &aux1);
        void            DeleteDataServer(std::string search, std::string &origin);
        int             nbrOccurrence(std::string &file, std::string search);
        void            SaveListen(CFGcluster *aux);
        void            ConfigMethod(std::string data, CFGcluster &cfg);
        void            checkPorts(void);
        std::string     saveLocationName(void);
        void            checkListen(CFGcluster *aux);
        void            SetDefaultServer();
        bool            CheckEmptyFile(void);

    public:
        std::vector<CFGcluster>         SrvCfg;
        
        SrvConfigFile(void);
        SrvConfigFile(std::string infile);
        SrvConfigFile(SrvConfigFile const &copy);
        ~SrvConfigFile(void);
        SrvConfigFile&  operator=(SrvConfigFile &pointer);
        void            checkRead(std::string data);

        void            printDataCluster(void);
        int             getnbrSrv(void);
};

#endif