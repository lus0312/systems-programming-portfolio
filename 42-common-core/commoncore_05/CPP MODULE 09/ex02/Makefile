NAME = PmergeMe
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -MMD -MP -O2

SRCS = main.cpp PmergeMe.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS)
fclean: clean
	rm -f $(NAME)
re: fclean
	$(MAKE) all

-include $(DEPS)

.PHONY: all clean fclean re