#include <iostream>
#include <string>
#include <SDL.h>
#include "cleanup.h"
#include "res_path.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

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
	SDL_Texture* texture = IMG_LoadTexture(ren, filepath.c_str());
	if (texture == nullptr) {
		logSDLError(std::cout, "LoadTexture");
	}
	return texture;
}

//void renderTexture(SDL_Texture* texture, SDL_Renderer* ren, int x, int y, int w, int h) {
//	SDL_Rect rect;
//	rect.x = x;
//	rect.y = y;
//	rect.w = w;
//	rect.h = h;
//	SDL_RenderCopy(ren, texture, NULL, &rect);
//  }

void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, SDL_Rect rect, SDL_Rect* clip = nullptr) {
	SDL_RenderCopy(ren, tex, clip, &rect);
}

void renderTexture(SDL_Texture* texture, SDL_Renderer* ren, int x, int y, SDL_Rect* clip = nullptr) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr) {
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(texture, ren, dst, clip);
}

SDL_Texture* renderText(const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* renderer) {
	TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (!font) {
		logSDLError(std::cout, "TTF_OpenFont");
		return nullptr;
	}

	SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (!surf) {
		TTF_CloseFont(font);
		logSDLError(std::cout, "TTF_RenderText_Blended");
		return nullptr;
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
	if (!tex) {
		logSDLError(std::cout, "CreateTextureFromSurface");
	}

	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return tex;
}

int main(int, char**)
{
	if (TTF_Init() != 0) {
		logSDLError(std::cout, "TTF_Init");
		SDL_Quit();
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Lesson 6", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

	const std::string resPath = getResourcePath("Lesson6");

	SDL_Color color = { 255,255,255,255 };
	SDL_Texture *text = renderText("TTF fonts are cool!", resPath + "sample.ttf", color, 64, renderer);

	//Make sure image loaded
	if (text == nullptr) {
		cleanup(renderer, window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}


	//clip
	int text_width, text_height;

	SDL_QueryTexture(text, NULL, NULL, &text_width, &text_height);

	// draws centered foreground image
	int x = (SCREEN_WIDTH - text_width) / 2;
	int y = (SCREEN_HEIGHT - text_height) / 2;

	SDL_Event e;
	bool quit = false;
	while (!quit) {
		//Event Polling
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
			}
		}
		SDL_RenderClear(renderer);
		//We can draw our message as we do any other texture, since it's been
		//rendered to a texture
		renderTexture(text, renderer, x, y);
		SDL_RenderPresent(renderer);
	}


	cleanup(renderer, window);
	TTF_Quit();
	SDL_Quit();
	return 0;
}