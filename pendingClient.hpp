#ifndef PENDINGCLIENT_HPP
#define PENDINGCLIENT_HPP

#include "client.hpp"

class PendingClient{
	private:
		std::string nickname;
		std::string username;
		std::string password;
		int users_fd;

		bool password_set;
		bool nickname_set;
		bool username_set;

		bool username_valid;
		bool nickname_valid;
		bool password_valid;

	public:
		PendingClient(int fd);

		int get_fd() const;
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getPassword() const;

		bool get_username_set() const;
		bool get_nickname_set() const;
		bool get_password_set() const;
		bool get_username_valid() const;
		bool get_nickname_valid() const;
		bool get_password_valid() const;

		void setNickname(std::string& nickname);
		void setUsername(std::string& username);
		void setPassword(std::string& password);

		bool isNicknameSet() const;
		bool isUsernameSet() const;
		bool isPasswordSet() const;

		bool checkPassword(std::string& password, std::string& truePassword) const;
		bool checkUsername(std::string& username, std::vector <User> users) const;
		bool checkNickname(std::string& nickname, std::vector <User> users) const;

		void handleRegistration(std::string& comand, std::string& password, std::vector <User> users);
		void handleUserComand(std::string& username, std::vector <User> users);
		void handleNickComand(std::string& nickname, std::vector <User> users);
		void handlePassComand(std::string& password, std::string& truePassword);

	};

	std::vector<std::string> splitBySpace(const std::string& input);

#endif
