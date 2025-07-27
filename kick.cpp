#include "channel.hpp"
#include "user.hpp"
#include "replies.hpp"
#include "pendingClient.hpp"
#include "server.hpp"

void Server::kickCmd(std::vector<std::string> tokens, User* user){
	std::string command = parsse(tokens[0]);
	if (tokens.size() < 3){
		sendReply(user->get_fd(), ERR_NEEDMOREPARAMS(command));
		return ;
	}
	std::string channelName = parsse(tokens[1]);
	std::string name = parsse(tokens[2]);
	std::string delComment = "";
	if (tokens.size() == 4)
		delComment = parsse(tokens[3]);
	Channel* channel = getChannelName(channelName);
	if (!channel){
		sendReply(user->get_fd() ,ERR_NOSUCHCHANNEL(channelName));
		return ;
	}
	User* delUser = getDelUser(name);
	if (!delUser){
		sendReply(user->get_fd() ,ERR_NOSUCHNICK(user->getNickname()));
		return ;
	}
	channel->handleKickCommand(user, delUser, delComment);
}

void Channel::handleKickCommand(User* user, User* delUser, std::string& delComment){
	if (!isUserInChannel(user->get_fd())){
		sendReply(user->get_fd(), ERR_NOTONCHANNEL(user->getNickname(), name));
		return ;
	}
	if (!isOperator(user->get_fd())){
		sendReply(user->get_fd(), ERR_CHANOPRIVSNEEDED(name));
		return ;
	}
	if (!isUserInChannel(delUser->get_fd())){
		sendReply(user->get_fd(), ERR_USERNOTINCHANNEL(delUser->getNickname(), name));
		return ;
	}
	for (std::vector<int>::iterator it = users_fd.begin(); it != users_fd.end(); ++it){
		sendReply(it[0], RPL_KICK(user->getNickname(), name, delUser->getNickname(), delComment));
	}
	removeUser(delUser->get_fd());
	delUser->leaveChannel(name);
}

void Channel::removeUser(int user_fd) {
    std::vector<int>::iterator user_vector;
    user_vector = std::find(users_fd.begin(), users_fd.end(), user_fd);
    if (user_vector != users_fd.end())
        users_fd.erase(user_vector);
    user_vector = std::find(operators_fd.begin(), operators_fd.end(), user_fd);
    if (user_vector != operators_fd.end())
        operators_fd.erase(user_vector);
    user_vector = std::find(inviteds_fd.begin(), inviteds_fd.end(), user_fd);
    if (user_vector != inviteds_fd.end())
        inviteds_fd.erase(user_vector);
	if (currentUsers > 0)
		currentUsers--;
}
