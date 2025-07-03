#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "user.hpp"

class Channel{
    private:
        std::string name;
        std::string key;
        std::string topic;
        std::vector <int> users_fd;
        std::vector <int> inviteds_fd;
        std::vector <int> operators_fd;
    public:
        Channel(const std::string& name, const std::string& key = "");

        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::string& getKey() const;

        void setTopic(const std::string& topic);

        void addUser(int users_fd, const std::string& key);
        void removeUser(int users_fd);
        bool isUserInChannel(int users_fd) const;
        void addOperator(int users_fd);
        bool isOperator(int users_fd) const;
        void invite(int users_fd);
        bool isInvited(int users_fd) const;
        bool hasKey() const;
        bool checkKey(const std::string& key) const;

		void handleJoinCommand(User* user, std::string& key);
};

#endif
