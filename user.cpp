#include "server.hpp"
#include "user.hpp"

int User::get_fd() const
{
	return (user_fd);
}

std::string User::getNickname() const{
	return nickname;
}

std::string User::getUsername() const{
	return username;
}
