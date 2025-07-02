#ifndef SERVER_HPP
#define SERVER_HPP

#include "client.hpp"

class Server
{
	private :
		int server_fd;
		std::vector <Client> clients;
		std::string password;
		int port;
	public :
		Server(int port, std::string password);
		void start_server();
		Client *look_for(int fd);
};


#endif













