#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "user.hpp"

class Channel{
    private:
        std::string name;
        std::string key;
        std::string topic;
		bool justCreated;
		bool inviteOnly;
		bool requiredKey;
		int maxUsers;
		int currentUsers;
		bool topicRest;
        std::vector <int> users_fd;
        std::vector <int> inviteds_fd;
        std::vector <int> operators_fd;

    public:
        Channel(const std::string& name, const std::string& key = "");

        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::string& getKey() const;
		const  std::vector <int>& getUsers_fd() const;
		bool getTopicRest() const ;
		std::string getUserList(std::vector <User>& users) const;
		User* getUserByfd(int fd, std::vector <User>& users) const;

        void setTopic(const std::string& topic);
		void setTopicRest(bool is);
		void setMaxUsers(int max);
		void setKey(std::string& key);
		void setInviteOnly(bool is);

        void addUser(User* user, const std::string& key, std::vector <User>& users);
        void removeUser(int users_fd);
        bool isUserInChannel(int user_fd) const;
        void addOperator(int user_fd);
		void removeOperator(int user_fd);
        bool isOperator(int user_fd) const;
        void invite(int user_fd);
        bool isInvited(int user_fd) const;
		void addInvited(int fd);
        bool hasKey();
        bool checkKey(const std::string& key) const;
		bool isFull() const;
		bool checkOperators();

		void handleJoinCommand(User* user, std::string& key, std::vector <User>& users);
		void handleKickCommand(User* user, User* delUser, std::string& delComment);
		void handleSingleMode(std::vector<std::string> tokens, User* user, User* opUser, std::string mode);
		void handleInviteMode(std::vector<std::string> tokens, User* user, std::string mode);
		void handleTopicMode(std::vector<std::string> tokens, User* user, std::string mode);
		void handleLimitMode(std::vector<std::string> tokens, User* user, std::string mode);
		void handleKeyMode(std::vector<std::string> tokens, User* user, std::string mode);
		void handleOperatorMode(std::vector<std::string> tokens, User* user, User* opUser, std::string mode);

		void broad_cast_channel(std::string msg, User sender, std::vector<User> users);
};

#endif

