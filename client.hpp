#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <poll.h>
#include <cstring>

class Client
{
	private :
		std::string name;
		int client_fd;
	public :
		Client(int fd);
		int get_fd() const;
};

#endif