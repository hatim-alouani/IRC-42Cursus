#ifndef PENDINGCLIENT_HPP
#define PENDINGCLIENT_HPP

#include "user.hpp"

class PendingClient{
	private:
		std::string username;
		std::string hostname;
		std::string servername;
		std::string realname;
		std::string nickname;
		std::string password;
		std::string buffer;
		std::string client_ip;
		int user_fd;

		bool password_set;
		bool nickname_set;
		bool username_set;
		bool hostname_set;
		bool servername_set;
		bool realname_set;

		bool nickname_valid;
		bool password_valid;

	public:
		PendingClient(int fd);

		const std::string getUsername() const;
		const std::string getHostname() const;
		const std::string getServername() const;
		const std::string getRealname() const;
		const std::string getNickname() const;
		const std::string getPassword() const;
		const std::string getBuffer() const;
		int get_fd() const;
		bool get_username_set() const;
		bool get_hostname_set() const;
		bool get_servername_set() const;
		bool get_realname_set() const;
		bool get_nickname_set() const;
		bool get_password_set() const;
		bool get_nickname_valid() const;
		bool get_password_valid() const;
		std::string get_ip_client() const;

		void setBufferEmpty();
		void setBuffer(std::string buffer);
		void setUsername(std::string& username);
		void setHostname(std::string& username);
		void setServername(std::string& username);
		void setRealname(std::string& username);
		void setNickname(std::string& nickname);
		void setPassword(std::string& password);
		void set_client_ip(std::string &ip);

		bool isNicknameSet() const;
		bool isUsernameSet() const;
		bool isHostnameSet() const;
		bool isServernameSet() const;
		bool isRealnameSet() const;
		bool isPasswordSet() const;

		bool checkPassword(std::string& password, std::string& truePassword) const;
		bool checkNickname(std::string& nickname, std::vector <User> users, std::vector <PendingClient> pendingUsers) const;

		void handleRegistration(std::vector<std::string> tokens, std::string& password, std::vector <User> users, std::vector <PendingClient> pendingUsers);
		void handleUserCommand(std::string& username, std::string& hostname, std::string& servername, std::string& realname);
		void handleNickCommand(std::string& nickname, std::vector <User> users, std::vector <PendingClient> pendingUsers);
		void handlePassCommand(std::string& password, std::string& truePassword);

	};

	std::vector<std::string> splitBySpace(const std::string& input);
	std::vector<std::string> splitByComma(const std::string& input);
	std::vector<std::string> splitByLine(const std::string& input);
	bool checkChannelName(std::string& name);
	std::string& parsse(std::string& std);
	void sendReply(int fd, std::string reply);
	bool isValidNickname(const std::string& nick);

#endif
