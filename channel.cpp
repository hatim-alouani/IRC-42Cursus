#include "channel.hpp"

Channel::Channel(const std::string& name, const std::string& key) : name(name) , key(key), topic(""){
}

const std::string% Channel::getName() const{
    return this->name;
}

const std::string% Channel::getTopic() const{
    return this->topic;
}

const std::string% Channel::getKey() const{
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

bool isClientInChannel(int client_fd) const{
    return std::find(clients_fd.begin(), clients_fd.end(), client_fd)
}

bool addClient(int client_fd, const std::string& key){
    if (isClientInChannel(client_fd) || (hasKey() && !checkKey(key)))
        return flase;
    clients_fd.push_back(client_fd);
}

void Channel::removeClient(int client_fd) {
    std::vector<int>::iterator client_vector;
    client_vector = std::find(clients_fd.begin(), clients_fd.end(), client_fd);
    if (client_vector != clients_fd.end())
        clients_fd.erase(client_vector);
    client_vector = std::find(operators_fd.begin(), operators_fd.end(), client_fd);
    if (client_vector != operators_fd.end())
        operators_fd.erase(client_vector);
    client_vector = std::find(inviteds_fd.begin(), inviteds_fd.end(), client_fd);
    if (client_vector != inviteds_fd.end())
        inviteds_fd.erase(client_vector);
}

bool isOperator(int client_fd) const{
    return std::find(operators_fd.begin(), operators_fd.end(), client_fd);
}

void addOperator(int client_fd){
    if (!isOperator(client_fd))
        operators_fd.push_back(client_fd);
}

bool isInvited(int client_fd) const{
    return std::find(inviteds_fd.begin(), inviteds_fd.end(), client_fd);
}

void invite(int client_fd){
    if (!isInvited(client_fd))
        inviteds_fd.push_back(client_fd);
}
