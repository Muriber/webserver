/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CFGlocation.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaromero <jaromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 00:10:56 by jaromero          #+#    #+#             */
/*   Updated: 2023/12/05 13:09:47 by jaromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CFGLOCATION_HPP
#define CFGLOCATION_HPP

#include <iostream>
#include <vector>


class CFGlocation
{
    private:
        std::string                     name_location;
        std::string                     root;
        std::string                     index;
        std::string                     try_files;
        std::string                     allow_methods;
        std::string                     autoindex;

        std::string                     fastcgi_pass;
        std::string                     fastcgi_index;
        int                             nbrparam;
        std::vector<std::string>        fastcgi_param;

        unsigned int                    client_max_body_size;
        std::string                     upload_store;
        std::string                     upload_pass;
        int                             nbr_upload_set;
        std::vector<std::string>        upload_set_form_field;

    public:
        CFGlocation(void);
        CFGlocation(CFGlocation const &copy);
        ~CFGlocation();
        CFGlocation&    operator=(CFGlocation const &pointer);
        
        void    clear(void);
        void    printData(void);

        /*------- SETS ---------*/
        void        SetName(std::string data);
        void        SetRoot(std::string data);
        void        SetIndex(std::string data);
        void        SetTryFiles(std::string data);
        void        SetMethods(std::string data);
        void        SetAutoindex(std::string data);

        void        SetCGIpass(std::string data);
        void        SetCGIindex(std::string data);
        void        SetNbrparam(int data);
        void        SetNewCGIparam(std::string data);

        void        SetMaxBodySize(std::string data);
        void        SetUploadStore(std::string data);
        void        SetUploadPass(std::string data);
        void        SetNbrUploadSet(int data);
        void        SetUploadFormField(std::string data);

        /*------- GETS ---------*/

        std::string     getName(void);
        std::string     getRoot(void);
        std::string     getIndex(void);
        std::string     getTryFiles(void);
        std::string     getMethods(void);
        std::string     getAutoindex(void);
        
        std::string     getCGIpass(void);
        std::string     getCGIindex(void);
        int             getNbrparam(void);
        std::string     getCGIparam(int nbr);

        unsigned int    getMaxBodySize(void);
        std::string     getUploadStore(void);
        std::string     getUploadPass(void);
        int             getNbrUploadSet(void);
        std::string     getUploadFormField(int nbr);
};

#endif