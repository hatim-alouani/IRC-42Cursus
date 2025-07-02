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

#define DEF_PROTOCOL 0 /*the default protocol for IPv4 TCP, which is TCP itself.*/
#define MAX_PENDING 1 /*is the backlog queue size (max pending connections waiting).*/
#define BLOCK_WAIT -1 /*blocks indefinitely until at least one socket has some activity*/

class User
{
	private :
		std::string username;
		std::string nickname;
		int users_fd;
	public :
		User(int fd);
		int get_fd() const;
		std::string getUsername() const;
		std::string getNickname() const;
};


#endif
