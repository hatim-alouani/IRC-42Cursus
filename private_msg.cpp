#include "server.hpp"
#include "replies.hpp"
#include "user.hpp"

void	send_msg(int fd, std::string  msg)
{
	send(fd, msg.c_str(), msg.size(), 0);
}

std::vector<std::string> get_recs(std::string &receiversToken)
{
	std::vector<std::string> tokens;
	std::string tkn;
	size_t i = 0;
	while (i < receiversToken.length())
	{
		if (receiversToken[i] == ',')
		{
			if (!tkn.empty())
			{
				tokens.push_back(tkn);
				tkn.clear();
			}
		}
		else
			tkn += receiversToken[i];
		i++;
	}
	if (!tkn.empty())
		tokens.push_back(tkn);
	return (tokens);
}

void Server::to_user(User sender, std::string msg, std::string target, std::vector<User> users)
{
	std::string rep_msg;
	bool found = false;
	size_t i = 0;
	while(i < users.size())
	{
		if (target == users[i].getNickname())
		{
			found = true;
			break;
		}
		i++;
	}
	if (found == true)
	{
		rep_msg = RPL_AWAY(sender.getNickname(), sender.getUsername(), sender.getHostname(), msg, target);
		send_msg(users[i].get_fd(), rep_msg);
	}
	else
	{
		rep_msg = ERR_NOSUCHNICK(target);
		send_msg(sender.get_fd(), rep_msg);
	}
}

void Channel::broad_cast_channel(std::string msg, User sender, std::vector<User> users)
{
	std::string rep_msg;
	int i = 0;
	while (i < currentUsers)
	{
		User *u = getUserByfd(users_fd[i], users);
		if(u && u->getNickname() != sender.getNickname())
		{
			rep_msg = ":" + sender.getNickname() + "!~" + sender.getUsername() + "@" + sender.getHostname() + " PRIVMSG " + this->getName() + " :" + msg + "\r\n";
			send_msg(users_fd[i], rep_msg);
		}
		i++;
	}
}

void Server::to_channel(User sender, std::string msg,std::string target, std::vector <Channel> cha)
{
	std::string rep_msg;
	size_t i = 0;
	bool found = false;
	while (i < cha.size())
	{
		if (target == cha[i].getName())
		{
			found = true;
			break;
		}
		i++;
	}
	if (found == true)
	{
		if (cha[i].isUserInChannel(sender.get_fd()))
		{
			cha[i].broad_cast_channel(msg, sender, users);
		}
		else
		{
			rep_msg = ERR_USERNOTINCHANNEL(sender.getNickname(), cha[i].getName());
			send_msg(sender.get_fd(), rep_msg);
		}
	}
	else
	{
		rep_msg = ERR_NOSUCHCHANNEL(target);
		send_msg(sender.get_fd(), rep_msg);
	}
}

void Server::deal_with_bot(User sender, std::string msg)
{
	std::string bot_name = "bot";
	std::string rep_msg;
	if (!msg.empty() && msg[msg.size() - 1] == '\n')
		msg.erase(msg.size() - 1);
	if (!msg.empty() && msg[msg.size() - 1] == '\r')
		msg.erase(msg.size() - 1);
	if (msg == "TIME")
	{
		User *bot_use = this->getDelUser(bot_name);
		if (bot_use)
		{
			send_msg(bot_use->get_fd(), sender.getNickname());
		}
		else
		{
			rep_msg = ERR_NOSUCHNICK(bot_name);
			send_msg(sender.get_fd(), rep_msg);
		}
	}
	else
	{
		std::string invalid = msg + " is an invalid option for bot\n";
		rep_msg = RPL_PRIVMSG(bot_name , sender.getNickname(), invalid);
		send_msg(sender.get_fd(), rep_msg);
	}
}


void Server::handlePrivateMessage(std::vector<std::string> tokens, std::vector<User> &users, std::vector<Channel> &channels, User *sender)
{
	std::string msg;
	std::string target;
	std::vector<std::string> vec_recipients;

	if (tokens.size() < 3)
	{
		std::string rep_msg = ERR_NEEDMOREPARAMS(tokens[0]);
		send_msg(sender->get_fd(), rep_msg);
		return;
	}

	vec_recipients = get_recs(tokens[1]);

	if (!tokens[2].empty() && tokens[2][0] == ':')
		msg = tokens[2].substr(1);
	else
		msg = tokens[2];

	for (size_t i = 3; i < tokens.size(); ++i)
		msg += " " + tokens[i];

	size_t i = 0;
	while (i < vec_recipients.size())
	{
		target = vec_recipients[i];
		if (!target.empty() && target[0] == '#')
		{
			to_channel(*sender, msg, target, channels);
		}
		else
		{
			if (target == "bot")
				deal_with_bot(*sender, msg);
			else
				to_user(*sender , msg, target, users);
		}
		i++;
	}

}
