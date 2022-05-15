#include <iostream>
#include <stdio.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include "backend/Game.hpp"

#define FONT_PATH "font/Timeless.ttf"
#define GRID_WIDTH 5
#define GRID_HEIGHT 6
#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

void print_event_type(Uint32 event);

static Game game(5, 6);

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font = NULL;
static TTF_Font *font_small = NULL;
static SDL_Surface *letter_surfaces[26] = {};
static SDL_Texture *letter_textures[26] = {};
static SDL_Surface *popup_surface = NULL;
static SDL_Texture *popup_texture = NULL;
static SDL_Rect tile[GRID_HEIGHT][GRID_WIDTH + 1] = {};
static char tile_char[GRID_HEIGHT][GRID_WIDTH + 1] = {};
enum e_tile_color {
	BLACK = 0,
	GRAY,
	YELLOW,
	GREEN,
} static tile_color[GRID_HEIGHT][GRID_WIDTH] = {};
static int width = DEFAULT_WINDOW_WIDTH;
static int height = DEFAULT_WINDOW_HEIGHT;
static int tile_width;
static int tile_height;
static int board_width;
static int board_height;
static int xmargin = 10;
static int ymargin = 10;
static int cursory = 0;
static int cursorx = 0;

static void cleanup() {
	if (font != NULL)       TTF_CloseFont(font);
	if (font_small != NULL) TTF_CloseFont(font_small);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	// We assume the last allocated letter is only followed by unallocated ones
	for (int i = 0; i < 26 && letter_surfaces[i] != NULL; i++)
		SDL_FreeSurface(letter_surfaces[i]);
	for (int i = 0; i < 26 && letter_textures[i] != NULL; i++)
		SDL_DestroyTexture(letter_textures[i]);
	if (popup_surface != NULL) SDL_FreeSurface(popup_surface);
	if (popup_texture != NULL) SDL_DestroyTexture(popup_texture);
}

#define fail(msg) fail_(__FILE__, __LINE__, msg)
#define sdlfail() sdlfail_(__FILE__, __LINE__)
#define ttffail() ttffail_(__FILE__, __LINE__)
#define safeatexit(func) safeatexit_(__FILE__, __LINE__, func)
static void fail_(const char *file, int line, const char *msg) {
	fprintf(stderr, "%s:%d: %s\n", file, line, msg);
	exit(1);
}
static void sdlfail_(const char *file, int line) {
	fail_(file, line, SDL_GetError());
}
static void ttffail_(const char *file, int line) {
	fail_(file, line, TTF_GetError());
}
static void safeatexit_(const char *file, int line, void (*func)(void)) {
	if (atexit(func) != 0)
		fail_(file, line, "atexit could not register function");
}

static void render() {
	int ret;
	SDL_Rect dst = {};

	if (SDL_SetRenderDrawColor(renderer, 0x12, 0x12, 0x13, 0xff) < 0) sdlfail();
	if (SDL_RenderClear(renderer) < 0) sdlfail();
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			dst = tile[y][x];
			if (tile_color[y][x] == BLACK) {
				if (SDL_SetRenderDrawColor(renderer, 0x33, 0x33, 0x33, 0xff) < 0) sdlfail();
				SDL_RenderDrawRect(renderer, &dst);
				dst.x += 1;
				dst.y += 1;
				dst.w -= 2;
				dst.h -= 2;
			}
			ret = 0;
			switch (tile_color[y][x]) {
				case BLACK: ret = SDL_SetRenderDrawColor(renderer, 0x12, 0x12, 0x13, 0xff); break;
				case GRAY:  ret = SDL_SetRenderDrawColor(renderer, 0x3a, 0x3a, 0x3c, 0xff); break;
				case YELLOW:ret = SDL_SetRenderDrawColor(renderer, 0xb5, 0x9f, 0x3b, 0xff); break;
				case GREEN: ret = SDL_SetRenderDrawColor(renderer, 0x53, 0x8d, 0x4e, 0xff); break;
			}
			if (ret < 0) sdlfail();
			SDL_RenderFillRect(renderer, &dst);
			if (tile_char[y][x] >= 'a' && tile_char[y][x] <= 'z') {
				int idx = tile_char[y][x] - 'a';
				SDL_Texture *tex = letter_textures[idx];
				if (SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h) < 0) sdlfail();
				dst.x += (tile[y][x].w - dst.w) / 2;
				dst.y += (tile[y][x].h - dst.h) / 2;
				SDL_RenderCopy(renderer, tex, NULL, &dst);
			}
		}
	}
	// Game Over screen
	if (game.is_finished()) {
		if (popup_surface == NULL) {
			char str[256];
			if (game.is_won()) {
				snprintf(str,
				         256,
				         "You won! The words was %s.",
				         game.get_goal().c_str());
			} else {
				snprintf(str,
				         256,
				         "You lost :( The word was %s.",
				         game.get_goal().c_str());
			}
			popup_surface = TTF_RenderText_Shaded(
					font_small,
					str,
					(SDL_Color){0xff, 0xff, 0xff, 0xff},
					(SDL_Color){0x00, 0x00, 0x00, 0xdd}
				); if (popup_surface == NULL) ttffail();
			popup_texture = SDL_CreateTextureFromSurface(
					renderer,
					popup_surface
				); if (popup_texture == NULL) sdlfail();
		}
		if (SDL_QueryTexture(popup_texture, NULL, NULL, &dst.w, &dst.h) < 0) sdlfail();
		dst.x = (width - dst.w) / 2;
		dst.y = (height - dst.h) / 2;
		SDL_RenderCopy(renderer, popup_texture, NULL, &dst);
	}
	SDL_RenderPresent(renderer);
}

static void update_dimensions() {
	SDL_GetWindowSize(window, &width, &height);
	board_width = width - xmargin * 2;
	board_height = height - ymargin * 2;
	tile_width = board_width / GRID_WIDTH * 9 / 10;
	tile_height = (height - ymargin * 2) / GRID_HEIGHT * 9 / 10;
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			tile[y][x].x = xmargin + (board_width * (x * 2 + 1) / GRID_WIDTH - tile_width) / 2;
			tile[y][x].y = ymargin + (board_height * (y * 2 + 1) / GRID_HEIGHT - tile_height) / 2;
			tile[y][x].w = tile_width;
			tile[y][x].h = tile_height;
		}
	}
}

int main() {
	SDL_Event event;
	int ret;

	try {
		game.import_dictionary_file("words.txt");
	} catch (Game::FileImportFailedException& e) {
		fail("Failed to open dicionary file words.txt");
	}
	if (!game.dictionary_size()) {
		fail("Word dictionary is empty");
	}

	game.start_game();
	printf("Answer: %s\n", game.get_goal().c_str());

	if (SDL_Init(SDL_INIT_VIDEO) < 0) { sdlfail(); } safeatexit(SDL_Quit);
	if (TTF_Init() < 0)               { ttffail(); } safeatexit(TTF_Quit);
	                                                 safeatexit(cleanup);

	window = SDL_CreateWindow("Wordle",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			DEFAULT_WINDOW_WIDTH,
			DEFAULT_WINDOW_HEIGHT,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL); if (window == NULL) sdlfail();
	renderer = SDL_CreateRenderer(window, -1, 0); if (renderer == NULL) sdlfail();

	font =       TTF_OpenFont(FONT_PATH, 72); if (font == NULL) ttffail();
	font_small = TTF_OpenFont(FONT_PATH, 42); if (font == NULL) ttffail();
	char str[2];
	str[1] = '\0';
	for (int i = 0; i < 26; i++) {
		str[0] = 'A' + i;
		letter_surfaces[i] = TTF_RenderText_Solid(
				font,
				str,
		        (SDL_Color){0xff, 0xff, 0xff, 0xff}
			); if (letter_surfaces[i] == NULL) ttffail();
		letter_textures[i] = SDL_CreateTextureFromSurface(
				renderer,
				letter_surfaces[i]
			); if (letter_textures[i] == NULL) sdlfail();
	}

	SDL_SetWindowMinimumSize(window, 640, 480);
	update_dimensions();

	// Main Loop
	while ((ret = SDL_WaitEvent(&event)) && event.type != SDL_QUIT) {
		switch (event.type) {

		case SDL_WINDOWEVENT:
			switch (event.window.event) {
			// Where we render things
			case SDL_WINDOWEVENT_EXPOSED: render(); break;
			case SDL_WINDOWEVENT_RESIZED: update_dimensions(); render(); break;
			}
			break;
		
		case SDL_KEYDOWN:
			char c;
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE: exit(0); break;
			case SDLK_BACKSPACE:
				if (cursorx > 0 && cursory < GRID_HEIGHT) {
					cursorx--;
					tile_char[cursory][cursorx] = '\0';
					render();
				}
				break;
			case SDLK_RETURN: {
				std::string str(&tile_char[cursory][0]);
				if (cursory < GRID_HEIGHT && game.is_word_valid(str)) {
					const Guess &guess = game.guess_word(str);
					for (int i = 0; str[i] != '\0'; i++) {
						if (guess.is_valid(i))
							tile_color[cursory][i] = GREEN;
						else if (guess.is_misplaced(i))
							tile_color[cursory][i] = YELLOW;
						else
							tile_color[cursory][i] = GRAY;
					}
					cursorx = 0;
					cursory += 1;
					render();
				}
			} break;
			default:
				if ((int)event.key.keysym.sym >= (int)SDLK_a &&
					(int)event.key.keysym.sym <= (int)SDLK_z) {
					c = (int)event.key.keysym.sym - (int)SDLK_a + 'a';
					if (cursorx < GRID_WIDTH &&
							cursory < GRID_HEIGHT &&
							!game.is_finished()) {
						tile_char[cursory][cursorx] = c;
						cursorx++;
					}
					render();
				}
				break;
			}
		}
	}
	if (!ret) sdlfail();
	exit(0);
}