#include "pendingClient.hpp"
#include "server.hpp"

PendingClient::PendingClient(int fd){
	this->user_fd = fd;
	this->nickname = "";
	this->password = "";
	this->username = "";
	this->nickname_set = false;
	this->username_set = false;
	this->password_set = false;
	this->username_valid = false;
	this->nickname_valid = false;
	this->password_valid = false;
}

bool Server::isReadyForRegistration(std::string& buff, PendingClient* pending) const{
	std::string pwd = password;
	pending->handleRegistration(buff, pwd, users);
	if (pending->get_username_set() && pending->get_nickname_set() && pending->get_password_set() && pending->get_username_valid() && pending->get_nickname_valid() && pending->get_password_valid())
		return true;
	return false;
}

void PendingClient::handleRegistration(std::string& buff, std::string& password, std::vector <User> users){
	std::vector<std::string> tokens = splitBySpace(buff);
	if (tokens.empty()) {
		std::cout << "Empty command" << std::endl;
		return;
	}
	std::string command = tokens[0];
	if (command == "NICK") {
		handleNickCommand(tokens[1], users);
	}
	else if (command == "USER") {
		handleUserCommand(tokens[1], users);
	}
	else if (command == "PASS") {
		handlePassCommand(tokens[1], password);
	}
	else {
		std::cout << "Unknown command: " << command << std::endl;
	}
}

void PendingClient::handleUserCommand(std::string& username, std::vector <User> users){
	setUsername(username);
	if (isUsernameSet())
		this->username_set = true;
	if (checkUsername(username, users))
		this->username_valid = true;
}

void PendingClient::handleNickCommand(std::string& nickname, std::vector <User> users){
	setNickname(nickname);
	if (isNicknameSet())
		this->nickname_set = true;
	if (checkNickname(nickname, users))
		this->nickname_valid = true;
}

void PendingClient::handlePassCommand(std::string& password, std::string& truePassword){
	setPassword(password);
	if (isPasswordSet())
		this->password_set = true;
	if (checkPassword(password, truePassword))
		this->password_valid = true;
}

void PendingClient::setUsername(std::string& username){
	this->username = username;
}

void PendingClient::setNickname(std::string& nickname){
	this->nickname = nickname;
}

void PendingClient::setPassword(std::string& password){
	this->password = password;
}

bool PendingClient::isUsernameSet() const{
	if (getUsername() != "")
		return true;
	return false;
}

bool PendingClient::isNicknameSet() const{
	if (getNickname() != "")
		return true;
	return false;
}

bool PendingClient::isPasswordSet() const{
	if (getPassword() != "")
		return true;
	return false;
}

bool PendingClient::checkPassword(std::string& password, std::string& truePassword) const{
	if (truePassword == password)
		return true;
	return false;
}

bool PendingClient::checkUsername(std::string& username, std::vector <User> users) const{
	std::vector<User>::const_iterator it = users.begin();
    for (; it != users.end(); ++it) {
        if (it->getUsername() == username) {
            return false;
        }
    }
    return true;
}

bool PendingClient::checkNickname(std::string& nickname, std::vector <User> users) const{
	std::vector<User>::const_iterator it = users.begin();
    for (; it != users.end(); ++it) {
        if (it->getNickname() == nickname) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> splitBySpace(const std::string& input) {
	std::vector<std::string> tokens;
	std::string token;
	for (size_t i = 0; i < input.length(); ++i) {
		if (input[i] == ' ') {
			if (!token.empty()) {
				tokens.push_back(token);
				token.clear();
			}
		} else {
			token += input[i];
		}
	}
	if (!token.empty()) {
		tokens.push_back(token);
	}
	return tokens;
}

int PendingClient::get_fd() const
{
	return user_fd;
}

std::string PendingClient::getNickname() const{
	return nickname;
}

std::string PendingClient::getUsername() const{
	return username;
}

std::string PendingClient::getPassword() const{
	return password;
}

bool PendingClient::get_username_set()const{
	return username_set;
}
bool PendingClient::get_nickname_set()const{
	return nickname_set;
}
bool PendingClient::get_password_set()const{
	return password_set;
}
bool PendingClient::get_username_valid()const{
	return username_valid;
}
bool PendingClient::get_nickname_valid()const{
	return nickname_valid;
}
bool PendingClient::get_password_valid()const{
	return password_valid;
}
