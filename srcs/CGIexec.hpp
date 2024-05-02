/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIexec.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaromero <jaromero@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 19:13:35 by jaromero          #+#    #+#             */
/*   Updated: 2023/12/05 13:11:03 by jaromero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIEXEC_HPP
#define CGIEXEC_HPP

#include <iostream>
#include <stdio.h>
#include <map>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include "CFGlocation.hpp"
#include <ctime>
#include <signal.h>
#include <fcntl.h>
#include "Response.hpp"

class CGIexec
{
    private:

        std::string     cgi_type;
        std::string     cgi_path;
        std::string     output;
        int             nbr_arg;
        char            **args;
        bool            execState;
        void            ExecCGI(void);

    public:
    
        CGIexec(void);
        CGIexec(CFGlocation* location, std::string _type, std::string exec, std::map<std::string, std::string> args);
        CGIexec(CGIexec const &copy);
        ~CGIexec(void);
        CGIexec&    operator=(CGIexec const &pointer);

        void            SetNumberArguments(std::map<std::string, std::string> _args);
        void            SetType(std::string pathcgi, std::string _type);
        void            SetArguments(std::map<std::string, std::string> _args, std::string exec);
        std::string     getOutput(void);
        void            freeArguments(void);
};

#endif