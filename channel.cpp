#include "channel.hpp"
#include "user.hpp"

Channel::Channel(const std::string& name, const std::string& key) : name(name) , key(key), topic(""){
}

const std::string& Channel::getName() const{
    return this->name;
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

bool Channel::hasKey() const{
    return !this->key.empty();
}

bool Channel::checkKey(const std::string& key) const{
    return this->key == key;
}

bool Channel::isUserInChannel(int user_fd) const{
    return std::find(users_fd.begin(), users_fd.end(), user_fd) != users_fd.end();
}

void Channel::addUser(int user_fd, const std::string& key){
    if (isUserInChannel(user_fd)){
		std::cout << "User already in Channel : " << name << std::endl;
        return ;
	}
	if (hasKey() && !checkKey(key)){
		std::cout << "Key incorrect : " << name << std::endl;
        return ;
	}
    users_fd.push_back(user_fd);
	std::cout << "User added successfully" << std::endl;
}

void Channel::removeUser(int user_fd) {
    std::vector<int>::iterator user_vector;
    user_vector = std::find(users_fd.begin(), users_fd.end(), user_fd);
    if (user_vector != users_fd.end())
        users_fd.erase(user_vector);
    user_vector = std::find(operators_fd.begin(), operators_fd.end(), user_fd);
    if (user_vector != operators_fd.end())
        operators_fd.erase(user_vector);
    user_vector = std::find(inviteds_fd.begin(), inviteds_fd.end(), user_fd);
    if (user_vector != inviteds_fd.end())
        inviteds_fd.erase(user_vector);
}

bool Channel::isOperator(int user_fd) const{
    return std::find(operators_fd.begin(), operators_fd.end(), user_fd) != users_fd.end();
}

void Channel::addOperator(int user_fd){
    if (!isOperator(user_fd))
        operators_fd.push_back(user_fd);
}

bool Channel::isInvited(int user_fd) const{
    return std::find(inviteds_fd.begin(), inviteds_fd.end(), user_fd) != users_fd.end();
}

void Channel::invite(int user_fd){
    if (!isInvited(user_fd))
        inviteds_fd.push_back(user_fd);
}

void Channel::handleJoinCommand(User* user, std::string& key){
	addUser(user->get_fd(), key);
}
