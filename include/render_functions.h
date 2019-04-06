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
    if (texture == nullptr){
        logSDLError(std::cout, "LoadTexture");
    }
	return texture;
}

void renderTexture(SDL_Texture* texture, SDL_Renderer* ren, int x, int y, int w, int h){
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
	renderTexture(tex,ren,x,y,w,h);
}