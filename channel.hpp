#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "server.hpp"

class Channel{
    private:
        std::string name;
        std::string key;
        std::string topic;
        std::vector <int> clients_fd;
        std::vector <int> inviteds_fd;
        std::vector <int> operators_fd;
    public:
        Channel(const std::string& name, const std::string& key = "");
        const std::string& getName() const;
        const std::string& getTopic() const;
        void setTopic(const std::string& topic);
        const std::string& Channel::getKey() const;
        bool addClient(int client_fd, const std::string& key);
        void removeClient(int client_fd);
        bool isClientInChannel(int client_fd) const;
        void addOperator(int client_fd);
        bool isOperator(int client_fd) const;
        void invite(int client_fd);
        bool isInvited(int client_fd) const;
        bool hasKey() const;
        bool checkKey(const std::string& key) const;
};

#endif