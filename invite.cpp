#include "channel.hpp"
#include "user.hpp"
#include "replies.hpp"
#include "pendingClient.hpp"
#include "server.hpp"

void Server::inviteCmd(std::vector<std::string> tokens, User* user){
	std::string command = parsse(tokens[0]);
	if (tokens.size() != 3){
		sendReply(user->get_fd(), ERR_NEEDMOREPARAMS(command));
		return ;
	}
	std::string nickname = parsse(tokens[1]);
	std::string channelname = parsse(tokens[2]);
	Channel* channel = getChannelName(channelname);
	if (!channel){
		sendReply(user->get_fd(), ERR_NOSUCHCHANNEL(channelname));
		return ;
	}
	if (!(channel->isUserInChannel(user->get_fd()))){
		sendReply(user->get_fd(), ERR_NOTONCHANNEL(user->getNickname(), channel->getName()));
		return ;
	}
	if (!(channel->isOperator(user->get_fd()))){
		sendReply(user->get_fd(), ERR_CHANOPRIVSNEEDED(channel->getName()));
		return ;
	}
	User* invitedUser = getDelUser(nickname);
	if (!invitedUser){
		sendReply(user->get_fd(), ERR_NOSUCHNICK(nickname));
		return ;
	}
	if (channel->isInvited(invitedUser->get_fd())){
		sendReply(user->get_fd(), ERR_ALREADYINVITED(nickname, channel->getName()));
		return ;
	}
	channel->addInvited(invitedUser->get_fd());
	sendReply(user->get_fd(), RPL_INVITING(user->getNickname(), nickname, channel->getName()));
	std::vector<int> users_fd = channel->getUsers_fd();
	for (std::vector<int>::iterator it = users_fd.begin(); it != users_fd.end(); ++it){
		sendReply(it[0], RPL_INVITE(user->getNickname(), nickname, channel->getName()));
	}
}
