#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <poll.h>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <set>

class User
{
	private :
		int user_fd;
		std::string username;
		std::string hostname;
		std::string servername;
		std::string realname;
		std::string nickname;
		int nbrChannels;
		std::string buffer;
		std::set<std::string> joinedChannels;
	public :
		User(int fd, std::string username, std::string hostname, std::string servername, std::string realname, std::string nickname);

		int get_fd() const;
		const std::string getUsername() const;
		const std::string getHostname() const;
		const std::string getServername() const;
		const std::string getRealname() const;
		const std::string getNickname() const;
		const std::string getBuffer() const;
		int getNbrChannels() const;
		const std::set<std::string>& getChannels() const;

		void setBufferEmpty();
		void setBuffer(std::string buffer);
		void setUsername(std::string& username);
		void setHostname(std::string& username);
		void setServername(std::string& username);
		void setRealname(std::string& username);
		void setNickname(std::string& nickname);

		void plusChannel() ;
		void joinChannel(const std::string& name);
    	void leaveChannel(const std::string& name);
};

#endif
