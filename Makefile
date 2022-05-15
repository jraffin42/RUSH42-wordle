CXX = clang++
CXXFLAGS += -Wall -Wextra -Werror

INC = backend
SRC = .
OBJDIR = obj

NAME = wordle
SRCS =  console_wordle.cpp backend/Game.cpp backend/Guess.cpp
OBJ =  $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.o))

$(OBJDIR)/%.o	:	$(SRC)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $(addprefix -I ,$(INC)) -o $@ $<

all: $(NAME)
$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

clean:
	rm -rf $(OBJDIR)
fclean: clean
	rm $(NAME)
re: fclean all
.PHONY: all clean fclean
