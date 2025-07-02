#include "parssing.hpp"

PendingClient::PendingClient(int fd){
	this->client_fd = fd;
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

bool Server::isReadyForRegistration(std::string& comand, PendingClient* pending) const{
	pending->handleRegistration(comand, password, clients);
	if (username_set && nickname_set && password_set && username_valid && nickname_valid && password_valid)
		return true;
	return false;
}

void PendingClient::handleRegistration(std::string& comand, std::string& password, std::vector <Client> clients){
	std::vector<std::string> tokens = splitBySpace(comand);
	if (tokens.empty()) {
		std::cout << "Empty command" << std::endl;
		return;
	}
	std::string command = tokens[0];
	else if (command == "NICK") {
		handleNickComand(token[1], clients);
	}
	else if (command == "USER") {
		handleUserComand(token[1], clients);
	}
	else if (command == "PASS") {
		handlePassComand(token[1], password);
	}
	else {
		std::cout << "Unknown command: " << command << std::endl;
	}
}

void PendingClient::handleUserComand(std::string username, std::vector <Client> clients){
	setUsername(username);
	if (isUsernameSet())
		this->username_set = true;
	if (checkUsername(username, clients))
		this->username_valid = true;
}

void PendingClient::handleNickComand(std::string nickname, std::vector <Client> clients){
	setNickname(nickname);
	if (isNicknameSet())
		this->nickname_set = true;
	if (checkNickname(Nickname, clients))
		this->nickname_valid = true;
}

void PendingClient::handleUserComand(std::string password, std::string truePassword){
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

bool PendingClient::isUsernameSet(){
	if (getUsername() != "")
		return true;
	return false;
}

bool PendingClient::isNicknameSet(){
	if (getNickname() != "")
		return true;
	return false;
}

bool PendingClient::isPasswordSet(){
	if (getPassword() != "")
		return true;
	return false;
}

bool PendingClient::checkPassword(std::string& password, std::string& truePassword) const{
	if (truePassword == password)
		return true;
	return false;
}

bool PendingClient::checkUsername(std::string& username, std::vector <Client> clients){
	std::vector<Client>::const_iterator it = clients.begin();
    for (it; it != clients.end(); ++it) {
        if (it->getUserName() == username) {
            return false;
        }
    }
    return true;
}

bool PendingClient::checkNickname(std::string& nickname, std::vector <Client> clients){
	std::vector<Client>::const_iterator it = clients.begin();
    for (it; it != clients.end(); ++it) {
        if (it->getNickName() == nickname) {
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

