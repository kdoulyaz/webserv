CFLAGS = -Wall -Werror -Wextra -I include -fsanitize=address -g -std=c++98
CPP = c++
SRC := $(shell find $(SRCS_DIR) -type f -name "*.cpp")
NAME = server
CLIENT = client

all: $(NAME)

SRCS := $(shell find $(SRCS_DIR) -type f -name "*.cpp")

# Convert source file paths to object file names in the obj directory
OBJS = $(patsubst $(shell find $(SRCS_DIR) -type f -name "*.cpp") \
/%.cpp, $(OBJS_DIR)/%.o, $(SRCS))

# Find subdirectories in the source directory
SUBDIRS := $(shell find $(SRCS_DIR) -type d)

$(NAME): $(OBJS_DIR) $(OBJS)
	@$(CPP) $(CFLAGS) $(OBJS) -o $(NAME)
	@$(PRINTF) "\r%100s\r$(BLUE)$(NAME) is up to date!$(DEFAULT)\n"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@$(foreach dir,$(SUBDIRS),mkdir -p $(patsubst $(SRCS_DIR)/%,$(OBJS_DIR)/%,$(dir));)

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.cpp 
	@$(CPP) $(CFLAGS) -c $< -o $@
	@$(PRINTF) "\rCompiling $(BLUE)$<$(DEFAULT)..."

clean:

fclean : clean
	@rm -rf $(NAME)
	@rm -rf $(CLIENT)


re : fclean all
		@clear

.PHONY: all clean fclean re 
