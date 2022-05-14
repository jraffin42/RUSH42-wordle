CC =       clang++
CXX =      clang++
CXXFLAGS = -Wall -Wextra $(shell $(SDL2_CONFIG) --cflags)
LDLIBS =   $(shell $(SDL2_CONFIG) --static-libs)

NAME = wordle
SRC =  wordle.cpp
OBJ =  $(SRC:.cpp=.o)

DIR := $(shell pwd)
SDL2 =        SDL2-2.0.22
SDL2_CONFIG = local/bin/sdl2-config

all: $(NAME)
$(NAME): $(OBJ)
$(OBJ): $(SDL2_CONFIG)
clean:
	rm -vf $(OBJ)
fclean:
	@rm -vf $(OBJ)
	@rm -vf $(NAME)
	rm -rf local
	rm -rf $(SDL2)
re: fclean all
reme: clean all
.PHONY: all clean fclean

$(SDL2):
	@printf "Downloading SDL2...\n"
	@curl -fsSL https://libsdl.org/release/SDL2-2.0.22.tar.gz | tar -xz
$(SDL2_CONFIG): $(SDL2)
	@printf "Configuring SDL2...\n"
	@mkdir -p $(SDL2)/build && \
	     cd $(SDL2)/build && \
	     ../configure --prefix=$(DIR)/local >/dev/null
	@printf "Building SDL2...\n"
	@cd $(SDL2)/build && make -j >/dev/null
	@printf "Installing SDL2 locally...\n"
	@cd $(SDL2)/build && make install >/dev/null
