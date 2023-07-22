SRC = src/main.cpp src/configue.cpp src/server.cpp

CFLAGS =  -fsanitize=address -g #-std=c++98
CPP = c++
NAME = server
CLIENT = client

all: $(NAME)

$(NAME): $(SRC)
	@$(CPP) $(CFLAGS) $(SRC) -o $(NAME) 

clean:

fclean : clean
	@rm -rf $(NAME)
	@rm -rf $(CLIENT)


re : fclean all

client: client.cpp
	@$(CPP) $(CFLAGS) client.cpp -o client

run: 
	./server config/config_file.conf 
	

.PHONY: all clean fclean re