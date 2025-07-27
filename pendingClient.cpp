#include "pendingClient.hpp"
#include "server.hpp"
#include "replies.hpp"

PendingClient::PendingClient(int fd){
	this->user_fd = fd;
	this->nickname = "";
	this->password = "";
	this->username = "";
	this->nickname_set = false;
	this->username_set = false;
	this->password_set = false;
	this->nickname_valid = false;
	this->password_valid = false;
}

int PendingClient::get_fd() const
{
	return user_fd;
}

const std::string PendingClient::getNickname() const{
	return nickname;
}

const std::string PendingClient::getUsername() const{
	return username;
}

const std::string PendingClient::getHostname() const{
	return hostname;
}

const std::string PendingClient::getServername() const{
	return servername;
}

const std::string PendingClient::getRealname() const{
	return realname;
}

const std::string PendingClient::getPassword() const{
	return password;
}

void PendingClient::setUsername(std::string& username){
	this->username = (username);
}

void PendingClient::setNickname(std::string& nickname){
	this->nickname = (nickname);
}

void PendingClient::setHostname(std::string& hostname){
	this->hostname = (hostname);
}

void PendingClient::setServername(std::string& servername){
	this->servername = (servername);
}

void PendingClient::setRealname(std::string& realname){
	this->realname = realname;
}

void PendingClient::setPassword(std::string& password){
	this->password = password;
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

bool PendingClient::get_nickname_valid()const{
	return nickname_valid;
}
bool PendingClient::get_password_valid()const{
	return password_valid;
}

bool PendingClient::get_hostname_set() const{
	return hostname_set;
}

bool PendingClient::get_servername_set() const{
	return servername_set;
}

bool PendingClient::get_realname_set() const{
	return realname_set;
}

bool PendingClient::isUsernameSet() const{
	if (getUsername() != "")
		return true;
	return false;
}

bool PendingClient::isHostnameSet() const{
	if (getHostname() != "")
		return true;
	return false;
}

bool PendingClient::isServernameSet() const{
	if (getServername() != "")
		return true;
	return false;
}

bool PendingClient::isRealnameSet() const{
	if (getRealname() != "")
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
	if (truePassword == parsse(password)){
		return true;
	}
	sendReply(user_fd, ERR_PASSWDMISMATCH(password));
	return false;
}

bool isValidNickname(const std::string& nick) {
    if (nick.empty())
		return false;
    if (!isalpha(nick[0]) && nick[0] != '_')
		return false;
    for (size_t i = 1; i < nick.length(); ++i) {
        if (!isalnum(nick[i]) && nick[i] != '-' && nick[i] != '_') {
            return false;
        }
    }
    return true;
}

bool PendingClient::checkNickname(std::string& nickname, std::vector <User> users, std::vector <PendingClient> pendingUsers) const{
	std::vector<User>::const_iterator it = users.begin();
	 if (nickname.empty() || !isValidNickname(nickname)) {
        sendReply(user_fd, ERR_ERRONEUSNICKNAME(nickname));
        return false;
    }
    for (; it != users.end(); ++it) {
        if (it->getNickname() == nickname) {
			sendReply(user_fd, ERR_NICKNAMEINUSE(nickname));
            return false;
        }
    }
	std::vector<PendingClient>::const_iterator itt = pendingUsers.begin();
    for (; itt != pendingUsers.end(); ++itt) {
        if (itt->getNickname() == nickname) {
			sendReply(user_fd, ERR_NICKNAMEINUSE(nickname));
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
			token = parsse(token);
		}
	}
	if (!token.empty()) {
		tokens.push_back(token);
	}
	return tokens;
}

std::vector<std::string> splitByComma(const std::string& input) {
	std::vector<std::string> tokens;
	std::string token;
	for (size_t i = 0; i < input.length(); ++i) {
		if (input[i] == ',') {
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

std::vector<std::string> splitByLine(const std::string& input) {
	std::vector<std::string> tokens;
	std::string token;
	for (size_t i = 0; i < input.length(); ++i) {
		if (input[i] == '\n' || input[i] == '\r') {
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

void sendReply(int fd, std::string reply) {
    send(fd, reply.c_str(), strlen(reply.c_str()), 0);
}

std::string& parsse(std::string& str) {
    while (!str.empty() && (str[str.size() - 1] == '\n' || str[str.size() - 1] == '\r')) {
        str.erase(str.size() - 1);
    }
    return str;
}

bool checkChannelName(std::string& name){
	return ((name[0] == '#' || name[0] == '&') && name.size() > 1);
}

const std::string PendingClient::getBuffer() const{
	return buffer;
}

void PendingClient::setBuffer(std::string buffer){
	this->buffer += buffer;
}

void PendingClient::setBufferEmpty(){
	this->buffer = "";
}

void PendingClient::set_client_ip(std::string &ip)
{
	this->client_ip = ip;
}
std::string PendingClient::get_ip_client() const
{
	return (client_ip);
}
