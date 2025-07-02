#include "channel.hpp"

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
void Channel::setTopic(std::string& topic) const{
    this->topic = topic;
}

bool hasKey() const{
    return !this->key.empty();
}

bool checkKey(const std::string& key) const{
    return this->Key == key;
}

bool isClientInChannel(int users_fd) const{
    return std::find(users_fd.begin(), users_fd.end(), users_fd)
}

bool addClient(int users_fd, const std::string& key){
    if (isClientInChannel(users_fd) || (hasKey() && !checkKey(key)))
        return flase;
    users_fd.push_back(users_fd);
}

void Channel::removeClient(int users_fd) {
    std::vector<int>::iterator client_vector;
    client_vector = std::find(users_fd.begin(), users_fd.end(), users_fd);
    if (client_vector != users_fd.end())
        users_fd.erase(client_vector);
    client_vector = std::find(operators_fd.begin(), operators_fd.end(), users_fd);
    if (client_vector != operators_fd.end())
        operators_fd.erase(client_vector);
    client_vector = std::find(inviteds_fd.begin(), inviteds_fd.end(), users_fd);
    if (client_vector != inviteds_fd.end())
        inviteds_fd.erase(client_vector);
}

bool isOperator(int users_fd) const{
    return std::find(operators_fd.begin(), operators_fd.end(), users_fd);
}

void addOperator(int users_fd){
    if (!isOperator(users_fd))
        operators_fd.push_back(users_fd);
}

bool isInvited(int users_fd) const{
    return std::find(inviteds_fd.begin(), inviteds_fd.end(), users_fd);
}

void invite(int users_fd){
    if (!isInvited(users_fd))
        inviteds_fd.push_back(users_fd);
}
