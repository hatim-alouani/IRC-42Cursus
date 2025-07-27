#include "pendingClient.hpp"
#include "server.hpp"
#include "replies.hpp"

bool Server::isReadyForRegistration(std::vector<std::string> tokens, PendingClient* pending) const{
	std::string pwd = password;
	 if (tokens.empty()) {
        return false;
    }
	if (!pending->get_password_valid() && tokens[0] != "PASS"){
		sendReply(pending->get_fd(), ERR_PASSWDMISMATCH(pending->getNickname()));
		return false;
	}
	pending->handleRegistration(tokens, pwd, users, pending_users);
	if (pending->get_username_set() && pending->get_nickname_set() && pending->get_password_set()  && pending->get_nickname_valid() && pending->get_password_valid() && pending->get_hostname_set() && pending->get_servername_set() && pending->get_realname_set())
		return true;
	return false;
}

void PendingClient::handleRegistration(std::vector<std::string> tokens, std::string& password, std::vector <User> users, std::vector <PendingClient> pendingUsers){
	if (tokens.empty())
		return;
	std::string command = parsse(tokens[0]);
	if (command != "NICK" && command != "PASS" && command != "USER"){
		sendReply(user_fd, ERR_UNKNOWNCOMMAND(command));
		return ;
	}
	if (tokens.size() < 2){
		sendReply(user_fd, ERR_NEEDMOREPARAMS(command));
		return ;
	}
	std::string value = parsse(tokens[1]);
	if (command == "NICK" && tokens.size() == 2) {
		handleNickCommand(value, users, pendingUsers);
	}
	else if (command == "PASS" && tokens.size() == 2) {
		handlePassCommand(value, password);
	}
	else if (command == "USER") {
		if (tokens.size() >= 5){
			std::string value1 = parsse(tokens[2]);
			std::string value2 = parsse(tokens[3]);
			std::string value3 = parsse(tokens[4]);
			for (size_t i = 5; i < tokens.size(); ++i)
				value3 += " " + parsse(tokens[i]);
			handleUserCommand(value, value1, value2, value3);
		}
		else
			sendReply(user_fd, ERR_NEEDMOREPARAMS(command));
	}
	else if (command == "JOIN" || command == "KICK" || command == "PRIVMSG" || command == "MODE" || command == "TOPIC" || command == "INVITE"){
		sendReply(user_fd, ERR_NOTREGISTERED);
	}
	else {
		sendReply(user_fd, ERR_UNKNOWNCOMMAND(command));
	}
}

void PendingClient::handleUserCommand(std::string& username, std::string& hostname, std::string& servername, std::string& realname){
	setUsername(username);
	setHostname(hostname);
	setServername(servername);
	setRealname(realname);
	if (isUsernameSet())
		this->username_set = true;
	if (isHostnameSet())
		this->hostname_set = true;
	if (isServernameSet())
		this->servername_set = true;
	if (isRealnameSet())
		this->realname_set = true;
}

void PendingClient::handleNickCommand(std::string& nickname, std::vector <User> users, std::vector <PendingClient> pendingUsers){
	setNickname(nickname);
	if (isNicknameSet())
		this->nickname_set = true;
	if (checkNickname(nickname, users, pendingUsers))
		this->nickname_valid = true;
}

void PendingClient::handlePassCommand(std::string& password, std::string& truePassword){
	setPassword(password);
	if (isPasswordSet())
		this->password_set = true;
	if (checkPassword(password, truePassword)){
		this->password_valid = true;
	}
}

bool checkNick(std::string& nickname, std::vector <User> users){
	std::vector<User>::const_iterator it = users.begin();
    for (; it != users.end(); ++it) {
        if (it->getNickname() == nickname) {
			sendReply(it->get_fd(), ERR_NICKNAMEINUSE(nickname));
            return false;
        }
    }
    return true;
}

void Server::nickChangeCmd(std::vector<std::string> tokens, User* user) {
	if (tokens.size() < 1) {
        return;
    }
	std::string command = parsse(tokens[0]);
    if (tokens.size() < 2) {
        sendReply(user->get_fd(), ERR_NEEDMOREPARAMS(command));
        return;
    }
    std::string newNick = parsse(tokens[1]);
    std::string oldNick = user->getNickname();
    if (!checkNickname(newNick, user->get_fd())) {
        return;
    }
    std::string msg = ":" + oldNick + "!" + user->getUsername() + "@" + user->getHostname() + " NICK :" + newNick + "\r\n";
    const std::set<std::string>& userChannels = user->getChannels();
    for (std::set<std::string>::const_iterator it = userChannels.begin(); it != userChannels.end(); ++it) {
        Channel* ch = getChannel(*it);
        if (!ch) continue;
        const std::vector<int>& fds = ch->getUsers_fd();
        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i] != user->get_fd()) {
                sendReply(fds[i], msg);
            }
        }
    }
    sendReply(user->get_fd(), msg);
    user->setNickname(newNick);
}
