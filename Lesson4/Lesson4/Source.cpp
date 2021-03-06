#include <iostream>
#include <string>
#include <SDL.h>
#include "cleanup.h"
#include "res_path.h"
#include "SDL_image.h"


/*
@param os: the output stream to write the message to
@param msg: the error message to write
*/
void logSDLError(std::ostream &os, const std::string& msg) {
	os << msg << " error: " << SDL_GetError() << std::endl;
}

/* TEXTURE LOADING FUNCTION: image to texture
image -> SDL_LoadBMP() -> SDL_Surface -> SDL_CreateTextureFromSurface() - > SDL_Texture
@param filepath: the bmp image file to load
@param ren the renderer to load the texture onto
*/
SDL_Texture* loadTexture(const std::string& filepath, SDL_Renderer* ren) {
	SDL_Texture* texture = IMG_LoadTexture(ren, filepath.c_str());
	if (texture == nullptr) {
		logSDLError(std::cout, "LoadTexture");
	}
	return texture;
}

void renderTexture(SDL_Texture* texture, SDL_Renderer* ren, int x, int y, int w, int h) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_RenderCopy(ren, texture, NULL, &rect);
}

/* TEXTURE RENDERING FUNCTION: Draw texture on renderer
-no scaling
*/
void renderTexture(SDL_Texture* texture, SDL_Renderer* ren, int x, int y) {
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	renderTexture(texture, ren, x, y, w, h);
}


int main(int, char**)
{

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	const int SCREEN_WIDTH = 604;
	const int SCREEN_HEIGHT = 480;

	const int TILE_SIZE = 40;

	SDL_Window* window = SDL_CreateWindow("Lesson 3", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		SDL_Quit();
		return 1;
	}

	const std::string resPath = getResourcePath("Lesson4");
	SDL_Texture *image = loadTexture(resPath + "image.png", renderer);
	//Make sure image loaded
	if (image == nullptr) {
		cleanup(image, renderer, window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}


	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;
	renderTexture(image, renderer, x, y);


	SDL_Event e;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN) {
				quit = true;
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN) {
				quit = true;
			}
		}
		SDL_RenderClear(renderer); // encouraged for code reusability
		renderTexture(image, renderer, x, y);
		SDL_RenderPresent(renderer);
	}



	cleanup(image, renderer, window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}