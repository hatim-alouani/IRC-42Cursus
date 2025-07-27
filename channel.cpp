#include "channel.hpp"
#include "user.hpp"
#include "replies.hpp"
#include "pendingClient.hpp"

Channel::Channel(const std::string& name, const std::string& key) : name(name) , key(key), topic(""), justCreated(true), inviteOnly(false), requiredKey(false), maxUsers(MAX_GLOBAL_USERS), currentUsers(0), topicRest(false){
}

const std::string& Channel::getName() const{
    return this->name;
}

const  std::vector <int>& Channel::getUsers_fd() const{
	return users_fd;
}

const std::string& Channel::getTopic() const{
    return this->topic;
}

const std::string& Channel::getKey() const{
    return this->key;
}
void Channel::setTopic(const std::string& topic){
    this->topic = topic;
}

bool Channel::hasKey() {
    if (!this->key.empty()){
		requiredKey = true;
		return true;
	}
	requiredKey = false;
	return false;
}

bool Channel::checkKey(const std::string& key) const{
    return this->key == key;
}

bool Channel::isUserInChannel(int user_fd) const{
    return std::find(users_fd.begin(), users_fd.end(), user_fd) != users_fd.end();
}

bool Channel::isOperator(int user_fd) const {
    return std::find(operators_fd.begin(), operators_fd.end(), user_fd) != operators_fd.end();
}

void Channel::addOperator(int user_fd){
    if (!isOperator(user_fd)){
        operators_fd.push_back(user_fd);
		return ;
	}
}

void Channel::removeOperator(int user_fd){
	std::vector<int>::iterator user_vector = std::find(operators_fd.begin(), operators_fd.end(), user_fd);
    if (user_vector != operators_fd.end())
        operators_fd.erase(user_vector);
}

bool Channel::isInvited(int user_fd) const{
    return std::find(inviteds_fd.begin(), inviteds_fd.end(), user_fd) != inviteds_fd.end();
}

void Channel::invite(int user_fd){
    if (!isInvited(user_fd))
        inviteds_fd.push_back(user_fd);
}

bool Channel::isFull() const{
	return currentUsers >= maxUsers;
}

void Channel::addInvited(int fd){
	if (!isInvited(fd)){
		inviteds_fd.push_back(fd);
	}
}

bool Channel::getTopicRest() const {
	return topicRest;
}

void Channel::setTopicRest(bool is)  {
	topicRest = is;
}

void Channel::setInviteOnly(bool is) {
	inviteOnly = is;
}

void Channel::setMaxUsers(int max){
	maxUsers = max;
}
void Channel::setKey(std::string& key){
	this->key = key;
}

std::string Channel::getUserList(std::vector <User>& users) const {
    std::string userList;
    for (size_t i = 0; i < users_fd.size(); ++i) {
        User* user = getUserByfd(users_fd[i], users);
        if (!user)
            continue;
        if (isOperator(user->get_fd()))
            userList += "@";
        userList += user->getNickname();
        userList += " ";
    }
    if (!userList.empty())
        userList.pop_back();
    return userList;
}

User* Channel::getUserByfd(int fd, std::vector <User>& users) const {
	std::vector <User>::iterator it = users.begin();
	for (; it != users.end(); ++it)
		if (it->get_fd() == fd)
			return &(*it);
	return NULL;
}

bool Channel::checkOperators(){
	if (operators_fd.size() > 1)
		return true;
	return false;
}
