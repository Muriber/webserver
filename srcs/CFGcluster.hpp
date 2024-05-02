/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CFGcluster.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaromero <jaromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 23:12:35 by jaromero          #+#    #+#             */
/*   Updated: 2023/12/06 10:53:56 by jaromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CFGCLUSTER_HPP
# define CFGCLUSTER_HPP

#include <iostream>
#include <vector>
#include <map>
#include "CFGlocation.hpp"
#include <sstream>

class CFGcluster
{
    private:
        std::string                 srvName;
        std::string                 root;
        std::string                 index;
        std::string                 autoindex;
        std::string                 allow_methods;
        std::string                 retrn_page;
        std::map<int, std::string>  errorPages;
        int                         nbrErrorPages;
        unsigned int                client_max_body_size;
        int                         listen;
        bool                        def_server;
        bool                        virtual_server;
        bool                        get;
        bool                        post;
        bool                        del;
        bool                        retrn_state;
        int                         retrn_number;
        int                         nbr_location;
        std::vector<CFGlocation>    locations;

    public:
        CFGcluster(void);
        CFGcluster(CFGcluster const &copy);
        ~CFGcluster();

        CFGcluster& operator=(CFGcluster &pointer);
        void    printLocation(void);

        void    SetSrvNme(std::string _name);
        void    SetRoot(std::string _root);
        void    SetIndex(std::string _index);
        void    SetAutoindex(std::string _autoindex);
        void    SetAllowmethods(std::string _allowmethods);
        void    SetErrorPages(std::string data);
        void    SetNbrErrorPages(int data);
        void    SetListen(int _listen);
        void    SetDefaultServer(void);
        void    SetVirtualServer(void);
        void    SetGet(std::string  data);
        void    SetPost(std::string  data);
        void    SetDel(std::string  data);
        void    SetBodyBuffer(std::string data);
        void    SetNbrLocation(int  _nbr);
        void    SetNewLocation(CFGlocation _data);
        void    SetReturnPage(std::string _data);

        std::string     getSrvNme(void);
        std::string     getRoot(void);
        std::string     getIndex(void);
        std::string     getAutoindex(void);
        std::string     getAllowmethods(void);
        std::string     getErrorPagesPath(int nbr);
        std::string     getReturnPage(void);
        int             getReturnNumber(void);
        int             getNbrErrorPages(void);
        int             getListen(void);
        bool            getDefaultServer(void);
        bool            getVirtualServer(void);
        unsigned int    getBodyBuffer(void);
        int             getNbrLocation(void);
        bool            getReturnState(void);
        bool            getGet(void);
        bool            getPost(void);
        bool            getDel(void);
        CFGlocation     getLocations(int i);
        CFGlocation*    getLocation(std::string _name);

        std::vector<CFGcluster> *cluster;
};

std::string             nbrtostring(size_t);

# endif