/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIexec.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjimenez <bjimenez@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 19:14:34 by jaromero          #+#    #+#             */
/*   Updated: 2023/12/05 14:26:29 by bjimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIexec.hpp"


CGIexec::CGIexec(void)
{
    this->cgi_type.clear();
    this->cgi_path.clear();
    this->output.clear();
    this->nbr_arg = 0;
    this->args = 0;
    this->execState = false;
}

CGIexec::CGIexec(CGIexec const &copy)
{
    *this = copy;
}

CGIexec::CGIexec(CFGlocation* location, std::string _type, std::string exec, std::map<std::string, std::string> _args)
{
    this->execState = false;
    if (location == NULL)
    {
        this->output = "Undefined CGI config!!\n";
        return ;
    }
    this->SetNumberArguments(_args);
    if (location->getCGIpass() != "")
    {
        std::cout << "location: " << location->getCGIpass() << std::endl;
        this->SetType(location->getCGIpass(), _type);
    }
    else
    {
        this->output =  "CGI interpreter not found\n";
        return ;
    }
    this->SetArguments(_args, exec);
    this->execState = true;
    this->ExecCGI();
}

CGIexec::~CGIexec(void)
{
    if (this->execState == true)
        this->freeArguments();
}

void        CGIexec::freeArguments(void)
{
    for (int i = 0; i < nbr_arg + 3; i++)
        delete [] args[i];
    delete [] args;
}

CGIexec&    CGIexec::operator=(CGIexec const &pointer)
{
    this->cgi_type = pointer.cgi_type;
    this->cgi_path = pointer.cgi_path;
    this->output = pointer.output;
    this->nbr_arg = pointer.nbr_arg;
    this->args = new char*[this->nbr_arg + 3];
    for (int i = 0; i < nbr_arg + 3; i++)
    {
        this->args[i] = new char[std::strlen(pointer.args[i]) + 1];
        std::strcpy(this->args[i], pointer.args[i]);
    }
    return (*this);
}

void    CGIexec::SetNumberArguments(std::map<std::string, std::string> _args)
{
    std::map<std::string, std::string>::iterator x;

    x = _args.begin();
    this->nbr_arg = 0;
    for (; x != _args.end(); x++)
        this->nbr_arg++;
}

void    CGIexec::SetType(std::string pathcgi, std::string _type)
{
    this->cgi_type = _type;

    if (_type == "*.php")
        this->cgi_type = "php";
    else if (_type == "*.py")
        this->cgi_type = "python3";
    
    if (pathcgi != "")
        this->cgi_path = pathcgi;
}

void    CGIexec::SetArguments(std::map<std::string, std::string> _args, std::string exec)
{
    std::map<std::string, std::string>::iterator x;
    int     i;

    i = 0;
    this->args = new char*[nbr_arg + 3];
    this->args[i] = new char[this->cgi_type.length() + 1];
    std::strcpy(this->args[i], this->cgi_type.c_str());
    i++;
    this->args[i] = new char[exec.length() + 1];
    std::strcpy(this->args[i], exec.c_str());
    i++;
    x = _args.begin();
    for (; x != _args.end(); i++) {
        args[i] = new char[x->second.length() + 1];
        std::strcpy(this->args[i], x->second.c_str());
        if (!std::strcmp(this->args[i], "%2B"))
            std::strcpy(this->args[i], "+");
        x++;
    }
    this->args[i] = NULL;
}

std::string     CGIexec::getOutput(void)
{
    return (this->output);
}

void            CGIexec::ExecCGI(void)
{
    ssize_t bytes_read;
    char buffer[1024];
    int pipe_fd[2];
    int status;
    
    if (pipe(pipe_fd) == -1)
    {
        this->freeArguments();
        throw("Error on pipe");
    }
    fcntl(pipe_fd[0], F_SETFL, O_NONBLOCK);

    pid_t child_pid = fork();
    if (child_pid == -1)
    {
        this->freeArguments();
        throw("Error on fork");
    }
    if (child_pid == 0)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        execve(this->cgi_path.c_str(), args, nullptr);
        this->freeArguments();
        std::cout << "Error in execve... Cant exec CGI!\n";
        exit(1);
    }   
    else 
    {
        double wait_time = 20;

        time_t start = std::time(nullptr);
        close(pipe_fd[1]);   
        while (true)
        {
            time_t now = std::time(nullptr);
            double diff = now - start;
            if (diff >= wait_time)
            {
                kill(child_pid, SIGKILL);
				this->output.clear();
                this->freeArguments();
				throw(ErrorResponseException("508 Error: Loop Detected"));
                break;
            }
            bytes_read = read(pipe_fd[0], buffer, sizeof(buffer));
            buffer[bytes_read] = '\0';
			if (bytes_read > 0)
            	this->output.append(buffer);
			if (bytes_read == 0 && this->output.length() > 0)
				break;
        }
        waitpid(child_pid, &status, 0);
        close(pipe_fd[0]);
    }
}
