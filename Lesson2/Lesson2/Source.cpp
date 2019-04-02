#include <iostream>
#include <string>
#include <SDL.h>
#include "res_path.h"
#include "cleanup.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


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
	SDL_Texture* texture = nullptr; // initialize to nullptr to avoid dangling pointer, initialize outside of if else block for return value
	SDL_Surface* surf = SDL_LoadBMP(filepath.c_str()); //c_str() gives a C-string copy of a string

	if (surf != nullptr) { // for readability, use this instead of if (surf)
		texture = SDL_CreateTextureFromSurface(ren, surf);
		cleanup(surf);

		// incase error in creating texture
		if (texture == nullptr) {
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}

	return texture;
}

/* TEXTURE RENDERING FUNCTION: Draw texture on renderer
-preserves texture's width and height
*/
void renderTexture(SDL_Texture* texture, SDL_Renderer* ren, int x, int y) {
	SDL_Rect rectangle; // object on the stack? no need to delete
	rectangle.x = x;
	rectangle.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h); // this modifies rectange itself
	SDL_RenderCopy(ren, texture, NULL, &rectangle);
}



int main(int, char**)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Lesson 2", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}

	SDL_Renderer* ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		SDL_Quit();
		return 1;
	}

	const std::string resPath = getResourcePath("Lesson2");
	SDL_Texture* bg = loadTexture(resPath + "background.bmp", ren);
	SDL_Texture* fg = loadTexture(resPath + "image.bmp", ren);
	if (bg == nullptr || fg == nullptr) {
		logSDLError(std::cout, "loadTexture");
		cleanup(bg, fg, ren, window);
		SDL_Quit();
		return 1;
	}

	// draw tiled bg
	SDL_RenderClear(ren); // clears render window
	int halfWidth, halfHeight;
	SDL_QueryTexture(bg, NULL, NULL, &halfWidth, &halfHeight); // get size of bg
	
	renderTexture(bg, ren, 0, 0);
	renderTexture(bg, ren, 0, halfHeight);
	renderTexture(bg, ren, halfWidth, 0);
	renderTexture(bg, ren, halfWidth, halfHeight);

	// draw centered fg
	int fgWidth, fgHeight;
	SDL_QueryTexture(fg, NULL, NULL, &fgWidth, &fgHeight);

	renderTexture(fg, ren, (SCREEN_WIDTH - fgWidth) / 2, (SCREEN_HEIGHT - fgHeight) / 2);

	// display window
	SDL_RenderPresent(ren);
	SDL_Delay(2000);

	cleanup(bg, fg, ren, window);


	SDL_Quit();
	return 0;
}