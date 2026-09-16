NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP

INCLUDES = -I includes

# ==========================================
# 1. Mandatory (Server)
# ==========================================
OBJS_DIR = objs/

SRCS = srcs/main.cpp \
		srcs/Server.cpp \
		srcs/Client.cpp \
		srcs/Message.cpp \
		srcs/Channel.cpp \
		srcs/Controller.cpp

OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))
DEPS = $(OBJS:.o=.d)

# ==========================================
# 2. Rules
# ==========================================
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(OBJS_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean
	$(MAKE) all

-include $(DEPS)

.PHONY: all clean fclean re 
