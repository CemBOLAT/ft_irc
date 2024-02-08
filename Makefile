NAME := ircserv
BOT_NAME := turco

BOT_DIR := bot
SRC_DIR := src


BOTSRC := $(wildcard $(BOT_DIR)/*.cpp)
SRC := $(wildcard $(SRC_DIR)/*.cpp)

BOT_OBJ_DIR := bot_obj
OBJ_DIR := obj


BOT_OBJ := $(addprefix $(BOT_OBJ_DIR)/,$(notdir $(BOTSRC:.cpp=.o)))
OBJ := $(addprefix $(OBJ_DIR)/,$(notdir $(SRC:.cpp=.o)))

INC := -I include

CC := c++

CFLAGS := -Wall -Wextra -Werror -std=c++98 -g -ggdb

all: $(NAME)

bot: $(BOT_NAME)

$(BOT_NAME): $(BOT_OBJ)
	$(CC) $(CFLAGS) $(BOT_OBJ) -o $(BOT_NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(BOT_OBJ_DIR)/%.o: $(BOT_DIR)/%.cpp
	@mkdir -p $(BOT_OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BOT_OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(BOT_NAME)

re: fclean all

.PHONY: all clean fclean re $(NAME) $(BOT_NAME)
