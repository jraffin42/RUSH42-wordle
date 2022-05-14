CC =       clang++
CXX =      clang++
CXXFLAGS = -Wall -Wextra $(shell $(SDL2_CONFIG) --cflags)
LDLIBS =   $(shell $(SDL2_CONFIG) --static-libs) -lSDL2_ttf

NAME = wordle
SRC =  wordle.cpp
OBJ =  $(SRC:.cpp=.o)

DIR := $(shell pwd)
SDL2 =        SDL2-2.0.22
SDL2_TTF =    SDL2_ttf-2.0.18
SDL2_CONFIG = local/bin/sdl2-config
SDL2_TTF_A =  local/lib/libSDL2_ttf.la

all: $(NAME)
$(NAME): $(OBJ)
$(OBJ): $(SDL2_CONFIG) $(SDL2_TTF_A)
clean:
	rm -vf $(OBJ)
fclean: clean
	@rm -vf $(NAME)
	rm -rf local
	rm -rf $(SDL2)
	rm -rf $(SDL2_TTF)
re: fclean all
reme: clean all
.PHONY: all clean fclean SDL2

$(SDL2_CONFIG):
	@printf "Downloading SDL2...\n"
	@curl -fsSL https://libsdl.org/release/SDL2-2.0.22.tar.gz | tar -xz
	@printf "Configuring SDL2...\n"
	@mkdir -p $(SDL2)/build && \
	     cd $(SDL2)/build && \
	     ../configure --prefix=$(DIR)/local --disable-shared --disable-dependency-tracking #>/dev/null
	@printf "Building SDL2...\n"
	@cd $(SDL2)/build && make -j4
	@printf "Installing SDL2 locally...\n"
	@cd $(SDL2)/build && make install >/dev/null

$(SDL2_TTF_A):
	@printf "Downloading SDL2_ttf...\n"
	@curl -fsSL https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.0.18/SDL2_ttf-2.0.18.tar.gz | tar -xz
	@printf "Configuring SDL2_ttf...\n"
	@mkdir -p $(SDL2_TTF)/build && \
	     cd $(SDL2_TTF)/build && \
	     ../configure --prefix=$(DIR)/local --disable-shared --disable-dependency-tracking #>/dev/null
	@printf "Building SDL2_ttf...\n"
	@cd $(SDL2_TTF)/build && make -j4
	@printf "Installing SDL2_ttf locally...\n"
	@cd $(SDL2_TTF)/build && make install >/dev/null