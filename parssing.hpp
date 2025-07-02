#ifndef PARSSING_HPP
#define PARSSING_HPP

class PendingClient{
	private:
		std::string nickname;
		std::string username;
		std::string password;
		int client_fd;

		bool password_set;
		bool nickname_set;
		bool username_set;

		bool username_valid;
		bool nickname_valid;
		bool password_valid;

	public:
		PendingClient(int fd);

		int get_fd() const;
		std::string getNickname(std::string& nickname) const;
		std::string getUsername(std::string& username) const;
		std::string getPassword(std::string& password) const;

		void setNickname(std::string& nickname);
		void setUsername(std::string& username);
		void setPassword(std::string& password);

		bool isNicknameSet() const;
		bool isUsernameSet() const;
		bool isPasswordSet() const;

		bool checkPassword(std::string& password) const;
		bool checkUsername(std::string& username) const;
		bool checkNickname(std::string& nickname) const;

		void handleRegistration(std::string comand);
		void handleUserComand(std::string username);
		void handleNickComand(std::string username);
		void handlePassComand(std::string username);

		bool isReadyForRegistration(std::string& comand) const;
	};

	std::vector<std::string> splitBySpace(const std::string& input);

#endif
