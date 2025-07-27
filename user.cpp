#include "server.hpp"
#include "user.hpp"
#include "replies.hpp"

int User::get_fd() const{
	return user_fd;
}

User::User(int fd, std::string username, std::string hostname, std::string servername, std::string realname, std::string nickname) : user_fd(fd) , username(username), hostname(hostname), servername(servername), realname(realname), nickname(nickname), nbrChannels(0){
}

const std::string User::getNickname() const{
	return nickname;
}

const std::string User::getUsername() const{
	return username;
}

const std::string User::getHostname() const{
	return hostname;
}

const std::string User::getServername() const{
	return servername;
}

const std::string User::getRealname() const{
	return realname;
}

void User::setUsername(std::string& username){
	this->username = (username);
}

void User::setNickname(std::string& nickname){
	this->nickname = (nickname);
}

void User::setHostname(std::string& hostname){
	this->hostname = (hostname);
}

void User::setServername(std::string& servername){
	this->servername = (servername);
}

void User::setRealname(std::string& realname){
	this->realname = realname;
}

int User::getNbrChannels() const{
	return nbrChannels ;
}

void User::plusChannel() {
	nbrChannels++ ;
}

const std::string User::getBuffer() const{
	return buffer;
}

void User::setBuffer(std::string buffer){
	this->buffer += buffer;
}

void User::setBufferEmpty(){
	this->buffer = "";
}

const std::set<std::string>& User::getChannels() const {
    return this->joinedChannels;
}

void User::joinChannel(const std::string& name) {
    this->joinedChannels.insert(name);
}

void User::leaveChannel(const std::string& name) {
    this->joinedChannels.erase(name);
}

