#include "channel.hpp"
#include "user.hpp"
#include "replies.hpp"
#include "pendingClient.hpp"
#include "server.hpp"
#include <vector>

void Server::joinCmd(std::vector<std::string> tokens, User* user) {
	std::vector<std::string> channelNames = splitByComma(parsse(tokens[1]));
	std::vector<std::string> keys;
	std::string command = tokens[0];
	if (tokens.size() < 2){
		sendReply(user->get_fd(), ERR_NEEDMOREPARAMS(command));
		return ;
	}
	if (tokens.size() >= 3)
		keys = splitByComma(parsse(tokens[2]));
	if (keys.size() > channelNames.size())
		return ;
	for (std::size_t i = 0; i < channelNames.size(); ++i) {
		std::string name = channelNames[i];
		std::string key;
		if (i >= keys.size())
			key = "";
		else
			key = keys[i];
		if (checkChannelName(name)){
			Channel* channel = getChannel(name, key);
			channel->handleJoinCommand(user, key, users);
		}
		else{
			sendReply(user->get_fd(), ERR_NEEDMOREPARAMS(command));
			return ;
		}
	}
}

void Channel::handleJoinCommand(User* user, std::string& key, std::vector <User>& users){
	addUser(user, key, users);
}

void Channel::addUser(User* user, const std::string& key, std::vector <User>& users){
    if (isUserInChannel(user->get_fd())){
		sendReply(user->get_fd(), ERR_USERONCHANNEL(user->getNickname(), name));
        return ;
	}
	if (hasKey() && !checkKey(key) && !isInvited(user->get_fd())){
		sendReply(user->get_fd(), ERR_BADCHANNELKEY(user->getNickname(), name));
        return ;
	}
	if (inviteOnly && !isInvited(user->get_fd())){
		sendReply(user->get_fd(), ERR_INVITEONLYCHAN(user->getNickname(), name));
		return ;
	}
	if (isFull()){
		sendReply(user->get_fd(), ERR_CHANNELISFULL(user->getNickname(), name));
		return ;
	}
	if (user->getNbrChannels() >= MAX_CHANNELS){
		sendReply(user->get_fd(), ERR_TOOMANYCHANNELS(name));
		return ;
	}
    users_fd.push_back(user->get_fd());
	currentUsers++;
	user->plusChannel();
	user->joinChannel(name);
	if (justCreated){
		addOperator(user->get_fd());
		justCreated = false;
	}
	if (topic != "")
		sendReply(user->get_fd(), RPL_TOPIC(user->getNickname(), name, topic));
	else
		sendReply(user->get_fd(), RPL_NOTOPIC(user->getNickname(), name));
	for (std::vector<int>::iterator it = users_fd.begin(); it != users_fd.end(); ++it){
		sendReply(it[0], RPL_JOIN(user->getNickname(), name));
	}
	sendReply(user->get_fd(), RPL_NAMREPLY(user->getNickname(), name, getUserList(users)));
	sendReply(user->get_fd(), RPL_ENDOFNAMES(user->getNickname(), name));
}
