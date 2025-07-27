#include "channel.hpp"
#include "user.hpp"
#include "replies.hpp"
#include "pendingClient.hpp"
#include "server.hpp"

void Server::topicCmd(std::vector<std::string> tokens, User* user){
	std::string command = parsse(tokens[0]);
	if (tokens.size() == 2 && tokens[1][0] == ':'){
		sendReply(user->get_fd(), ERR_NEEDMOREPARAMS(command));
		return ;
	}
	std::string channelname = parsse(tokens[1]);
	Channel* channel = getChannelName(channelname);
	if (!channel){
		sendReply(user->get_fd(), ERR_NOSUCHCHANNEL(command));
		return ;
	}
	if (!(channel->isUserInChannel(user->get_fd()))){
		sendReply(user->get_fd(), ERR_NOTONCHANNEL(user->getNickname(), channel->getName()));
		return ;
	}
	if (tokens.size() == 2){
		if (channel->getTopic() == ""){
			sendReply(user->get_fd(), RPL_NOTOPIC(user->getNickname(), channel->getName()));
			return ;
		}
		for (std::vector<int>::const_iterator it = (channel->getUsers_fd()).begin(); it != (channel->getUsers_fd()).end(); ++it){
			sendReply(*it, RPL_TOPIC(user->getNickname(), channel->getName(), channel->getTopic()));
		}
		return ;
	}
	std::string topic;
	for (size_t i = 2; i < tokens.size(); ++i) {
		topic += tokens[i];
		if (i + 1 < tokens.size())
			topic += " ";
	}
	if (!(channel->isOperator(user->get_fd())) && channel->getTopicRest()){
		sendReply(user->get_fd(), ERR_CHANOPRIVSNEEDED(channel->getName()));
		return ;
	}
	channel->setTopic(topic);
	for (std::vector<int>::const_iterator it = (channel->getUsers_fd()).begin(); it != (channel->getUsers_fd()).end(); ++it){
		sendReply(*it, RPL_TOPIC(user->getNickname(), channel->getName(), channel->getTopic()));
	}
}
