#include "server.hpp"

int User::get_fd() const
{
	return (users_fd);
}

User::User(int fd):users_fd(fd)
{
	/*init the rest of data later */
}

std::string User::getNickname() const{
	return nickname;
}

std::string User::getUsername() const{
	return username;
}
