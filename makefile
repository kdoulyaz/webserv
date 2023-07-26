CFLAGS = -Wall -Wextra -Werror -std=c++98 -Iinclude #-fsanitize=address -g
CPP = c++
NAME = server

SRCS = src/main.cpp src/configue.cpp src/webserv.cpp src/network.cpp
OBJS = $(patsubst src/%.cpp,obj/%.o,$(SRCS))

all: $(NAME)

$(NAME): obj_dir $(OBJS)
	$(CPP) $(CFLAGS) $(OBJS) -o $(NAME) 

obj_dir:
	mkdir -p obj

$(OBJS) : obj/%.o : src/%.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj

fclean : clean
	rm -rf $(NAME)


re : fclean all

.PHONY: all clean fclean re
