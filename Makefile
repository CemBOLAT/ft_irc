NAME := ircserv

SRC_DIR := src

SRC := $(wildcard $(SRC_DIR)/*.cpp)

OBJ_DIR := obj

OBJ := $(addprefix $(OBJ_DIR)/,$(notdir $(SRC:.cpp=.o)))

INC := -I include

CC := c++

CFLAGS := -Wall -Wextra -Werror -std=c++98 -g -ggdb

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
