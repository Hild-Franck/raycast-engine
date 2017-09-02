#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <string>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

TTF_Font *font;

SDL_Event event;

SDL_Color textColor = { 0, 0, 0 };

SDL_Surface *background = NULL;
SDL_Surface *message = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *down = NULL;
SDL_Surface *up = NULL;

SDL_Surface *load_image(std::string filename) {
	SDL_Surface *loadedImage = NULL;
	SDL_Surface *optimizedImage = NULL;

	loadedImage = IMG_Load(filename.c_str());

	if (loadedImage != NULL) {
		optimizedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
	}

	return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, NULL, destination, &offset);
}

bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		return false;
	}

	if (TTF_Init() == -1) {
		return false;
	}
	
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	if (screen == NULL) {
		return false;
	}

	SDL_WM_SetCaption("Hello World ! v1.0", NULL);

	return true;
}

bool load_files() {
	font = TTF_OpenFont("resources/geekt.ttf", 28);

	if (font == NULL) {
		return false;
	}

	background = load_image("resources/background.png");

	if (background == NULL) {
		return false;
	}

	return true;
}

void clean_up() {
	TTF_CloseFont(font);
	TTF_Quit();

	SDL_FreeSurface(background);
	SDL_FreeSurface(message);
	SDL_FreeSurface(down);
	SDL_FreeSurface(up);

	SDL_Quit();
}

int main(int argc, char* args[]) {
	bool quit = false;

	if (init() == false) {
		return 1;
	}
	if (load_files() == false) {
		return 1;
	}

	up = TTF_RenderText_Solid(font, "Haut a ete presse.", textColor);
	down = TTF_RenderText_Solid(font, "Bas a ete presse.", textColor);

	if (up == NULL || down == NULL) {
		return 1;
	}


	while (quit == false) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			} else if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_UP: message = up;
					break;
					case SDLK_DOWN: message = down;
					break;
				}
			}
			if (message != NULL) {
				apply_surface(0, 0, background, screen);
				apply_surface((SCREEN_WIDTH - message->w)/2, (SCREEN_HEIGHT - message->h)/2, message, screen);
				message = NULL;
			}
			if (SDL_Flip(screen) == -1) {
				return 1;
			}
		}
	}

	clean_up();

	return 0;
}