#include <iostream>
#include <stdio.h>
#include "SDL.h"
#include "SDL_ttf.h"

#define FONT_PATH "font/Timeless.ttf"
#define GRID_WIDTH 5
#define GRID_HEIGHT 6
#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480

void print_event_type(Uint32 event);

enum e_tile_color {
	BLACK = 0,
	GRAY,
	YELLOW,
	GREEN,
};

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font = NULL;
static SDL_Surface *letter_surfaces[26] = {};
static SDL_Texture *letter_textures[26] = {};
static SDL_Rect tile[GRID_HEIGHT][GRID_WIDTH] = {};
static char tile_char[GRID_HEIGHT][GRID_WIDTH] = {};
static e_tile_color tile_color[GRID_HEIGHT][GRID_WIDTH] = {};
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
	if (font != NULL)
		TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	// We assume the last allocated letter is only followed by unallocated ones
	for (int i = 0; i < 26 && letter_surfaces[i] != NULL; i++)
		SDL_FreeSurface(letter_surfaces[i]);
	for (int i = 0; i < 26 && letter_textures[i] != NULL; i++)
		SDL_DestroyTexture(letter_textures[i]);
}

static void quit(int status) {
	exit (status);
}

#define sdlfail() sdlfail_(__FILE__, __LINE__)
#define ttffail() ttffail_(__FILE__, __LINE__)
#define safeatexit(func) safeatexit_(__FILE__, __LINE__, func)
static void sdlfail_(const char *file, int line) {
	fprintf(stderr, "%s:%d: %s\n", file, line, SDL_GetError());
	exit(1);
}
static void ttffail_(const char *file, int line) {
	fprintf(stderr, "%s:%d: %s\n", file, line, TTF_GetError());
	exit(1);
}
static void safeatexit_(const char *file, int line, void (*func)(void)) {
	if (atexit(func) != 0) {
		fprintf(stderr, "%s:%d: atexit could not register function\n", file, line);
		exit(1);
	}
}

static void render() {
	int ret;
	SDL_Rect dst = {};

	ret = SDL_SetRenderDrawColor(renderer, 0x12, 0x12, 0x13, 0xff);
	if (ret < 0) sdlfail();
	ret = SDL_RenderClear(renderer);
	if (ret < 0) sdlfail();
	for (int y = 0; y < GRID_HEIGHT; y++) {
		for (int x = 0; x < GRID_WIDTH; x++) {
			dst = tile[y][x];
			if (tile_color[y][x] == BLACK) {
				ret = SDL_SetRenderDrawColor(renderer, 0x33, 0x33, 0x33, 0xff);
				if (ret < 0) sdlfail();
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
				SDL_Texture *tex = letter_textures[2];
				if (SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h) < 0) sdlfail();
				dst.x += (tile[y][x].w - dst.w) / 2;
				dst.y += (tile[y][x].h - dst.h) / 2;
				SDL_RenderCopy(renderer, letter_textures[0], NULL, &dst);
			}
		}
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
			tile_color[y][x] = (e_tile_color)(x % 4); // TODO: Remove this test initialization
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

	if (SDL_Init(SDL_INIT_VIDEO) < 0) sdlfail();
	safeatexit(SDL_Quit);
	if (TTF_Init() < 0) ttffail();
	safeatexit(TTF_Quit);

	safeatexit(cleanup);
	window = SDL_CreateWindow("Wordle",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 480,
		SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
	if (window == NULL) sdlfail();
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) sdlfail();

	font = TTF_OpenFont(FONT_PATH, 72); if (font == NULL) ttffail();
	char str[2];
	str[1] = '\0';
	for (int i = 0; i < 26; i++) {
		str[0] = 'A' + i;
		printf("str: %s\n", str);
		letter_surfaces[i] = TTF_RenderText_Solid(
				font, str,
		        (SDL_Color){0xff, 0xff, 0xff, 0xff});
		if (letter_surfaces[i] == NULL) ttffail();
		letter_textures[i] = SDL_CreateTextureFromSurface(
				renderer, letter_surfaces[i]);
		if (letter_textures[i] == NULL) sdlfail();
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
			case SDL_WINDOWEVENT_RESIZED: update_dimensions(); break;
			default: print_event_type(event.type);
			}
			break;
		
		case SDL_KEYDOWN:
			char c;
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE: exit(0); break;
			default:
				if ((int)event.key.keysym.sym >= (int)SDLK_a &&
					(int)event.key.keysym.sym <= (int)SDLK_z) {
					c = (int)event.key.keysym.sym - (int)SDLK_a + 'a';
					printf("TEST %d %d %c\n", cursorx, cursory, c);
					if (cursorx < GRID_WIDTH)
						tile_char[cursory][cursorx++] = c;
					if (cursorx == GRID_WIDTH && cursory < GRID_HEIGHT - 1) {
						cursory++;
						cursorx = 0;
					}
					render();
				}
				break;
			}

		default: print_event_type(event.type);
		}
	}
	if (!ret) { fprintf(stderr, "wordle: %s\n", SDL_GetError()); quit(1); }
	exit(0);
}

void print_event_type(Uint32 event) {
	switch (event) {
		case 0:     printf("SDL_FIRSTEVENT\n"); 				break;
		case 0x100: printf("SDL_QUIT\n"); 						break;
		case 0x101: printf("SDL_APP_TERMINATING\n"); 			break;
		case 0x102: printf("SDL_APP_LOWMEMORY\n"); 				break;
		case 0x103: printf("SDL_APP_WILLENTERBACKGROUND\n"); 	break;
		case 0x104: printf("SDL_APP_DIDENTERBACKGROUND\n"); 	break;
		case 0x105: printf("SDL_APP_WILLENTERFOREGROUND\n"); 	break;
		case 0x106: printf("SDL_APP_DIDENTERFOREGROUND\n"); 	break;
		case 0x107: printf("SDL_LOCALECHANGED\n"); 				break;
		case 0x150: printf("SDL_DISPLAYEVENT\n"); 				break;
		case 0x200: printf("SDL_WINDOWEVENT\n"); 				break;
		case 0x201: printf("SDL_SYSWMEVENT\n"); 				break;
		case 0x300: printf("SDL_KEYDOWN\n"); 					break;
		case 0x301: printf("SDL_KEYUP\n"); 						break;
		case 0x302: printf("SDL_TEXTEDITING\n"); 				break;
		case 0x303: printf("SDL_TEXTINPUT\n"); 					break;
		case 0x304: printf("SDL_KEYMAPCHANGED\n"); 				break;
		case 0x305: printf("SDL_TEXTEDITING_EXT,\n"); 			break;
		case 0x400: printf("SDL_MOUSEMOTION\n"); 				break;
		case 0x401: printf("SDL_MOUSEBUTTONDOWN\n"); 			break;
		case 0x402: printf("SDL_MOUSEBUTTONUP\n"); 				break;
		case 0x403: printf("SDL_MOUSEWHEEL\n"); 				break;
		case 0x600: printf("SDL_JOYAXISMOTION\n"); 				break;
		case 0x601: printf("SDL_JOYBALLMOTION\n"); 				break;
		case 0x602: printf("SDL_JOYHATMOTION\n"); 				break;
		case 0x603: printf("SDL_JOYBUTTONDOWN\n"); 				break;
		case 0x604: printf("SDL_JOYBUTTONUP\n"); 				break;
		case 0x605: printf("SDL_JOYDEVICEADDED\n"); 			break;
		case 0x606: printf("SDL_JOYDEVICEREMOVED\n"); 			break;
		case 0x650: printf("SDL_CONTROLLERAXISMOTION\n"); 		break;
		case 0x651: printf("SDL_CONTROLLERBUTTONDOWN\n"); 		break;
		case 0x652: printf("SDL_CONTROLLERBUTTONUP\n"); 		break;
		case 0x653: printf("SDL_CONTROLLERDEVICEADDED\n"); 		break;
		case 0x654: printf("SDL_CONTROLLERDEVICEREMOVED\n"); 	break;
		case 0x655: printf("SDL_CONTROLLERDEVICEREMAPPED\n"); 	break;
		case 0x656: printf("SDL_CONTROLLERTOUCHPADDOWN\n"); 	break;
		case 0x657: printf("SDL_CONTROLLERTOUCHPADMOTION\n"); 	break;
		case 0x658: printf("SDL_CONTROLLERTOUCHPADUP\n"); 		break;
		case 0x659: printf("SDL_CONTROLLERSENSORUPDATE\n"); 	break;
		case 0x700: printf("SDL_FINGERDOWN\n"); 				break;
		case 0x701: printf("SDL_FINGERUP\n"); 					break;
		case 0x702: printf("SDL_FINGERMOTION\n"); 				break;
		case 0x800: printf("SDL_DOLLARGESTURE\n"); 				break;
		case 0x801: printf("SDL_DOLLARRECORD\n"); 				break;
		case 0x802: printf("SDL_MULTIGESTURE\n"); 				break;
		case 0x900: printf("SDL_CLIPBOARDUPDATE\n"); 			break;
		case 0x1000: printf("SDL_DROPFILE\n"); 					break;
		case 0x1001: printf("SDL_DROPTEXT\n"); 					break;
		case 0x1002: printf("SDL_DROPBEGIN,\n"); 				break;
		case 0x1003: printf("SDL_DROPCOMPLETE,\n"); 			break;
		case 0x1100: printf("SDL_AUDIODEVICEADDED\n"); 			break;
		case 0x1101: printf("SDL_AUDIODEVICEREMOVED\n"); 		break;
		case 0x1200: printf("SDL_SENSORUPDATE\n"); 				break;
		case 0x2000: printf("SDL_RENDER_TARGETS_RESET\n"); 		break;
		case 0x2001: printf("SDL_RENDER_DEVICE_RESET\n"); 		break;
		case 0x7F00: printf("SDL_POLLSENTINEL\n"); 				break;
		case 0x8000: printf("SDL_USEREVENT\n"); 				break;
		case 0xFFFF: printf("SDL_LASTEVENT\n"); 				break;
		default: printf("wtf\n");
	}
}