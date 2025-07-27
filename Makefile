NAME = ircserv
NAME_BONUS = bot

SRC_FILES = user.cpp helpers.cpp main.cpp pendingClient.cpp server.cpp channel.cpp join.cpp kick.cpp authetification.cpp invite.cpp topic.cpp mode.cpp private_msg.cpp
SRC_FILES_BONUS = bonus/bot.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)
OBJ_FILES_BONUS = $(SRC_FILES_BONUS:.cpp=.o)

HEADERS = server.hpp user.hpp pendingClient.hpp channel.hpp replies.hpp

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98

all : $(NAME)

$(NAME) : $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $(NAME)

bonus : $(NAME_BONUS)

$(NAME_BONUS) : $(OBJ_FILES_BONUS)
	$(CC) $(CFLAGS) $(OBJ_FILES_BONUS) -o $(NAME_BONUS)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

%bonus/.o: %bonus/.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(OBJ_FILES)
	rm -f $(OBJ_FILES_BONUS)

fclean : clean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re: fclean all

.PHONY: clean
