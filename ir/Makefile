Name := ircserv

Src :=	main.cpp \
		./src/Client.cpp \
		./src/Server.cpp \
		./src/Room.cpp \
		./src/Utils.cpp \
	
Obj := $(Src:.cpp=.o)

all : $(Name)


$(Name) : $(Src)
	clang++ -Wall -Wextra -Werror -std=c++98 -g -o $(Name) $(Src)

clean :
	rm -f $(Obj)

fclean : clean
	rm -f $(Name)

re : fclean all

.PHONY : all clean fclean re