
#include "Text.h"

Text::Text(string content, float x, float y, Game* game) {
	this->content = content;
	this->x = x;
	this->y = y;
	this->game = game;
}

void Text::draw() {
	//Color de la fuente
	SDL_Color color;//Si ponemos un new, creamos la clase en el heap (un struct también está en el heap de C++), si no se usa el new, es en el stack. Los punteros también van en el stack????. Esto es importante porque todo lo del heap debemos borrarlo nosotros.
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255; //transparente->0

	//superficie y "textura" del texto
	SDL_Surface* surface = TTF_RenderText_Blended(game->font, content.c_str(), color);
	// c_str() transforma el string a cost *char-> c_str porqwue C no trabaja con cadenas, trabaja con arrays de caracteres;
	SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, surface);

	//Rectangulo donde queremos que se pinte
	SDL_Rect rect; // Base de coordenadas esquina superior izquierda
	rect.x = x - surface->w / 2;
	rect.y = y - surface->h / 2;
	rect.w = surface->w;
	rect.h = surface->h;

	SDL_FreeSurface(surface); //surface está en heap, hay que librerarlo (por culpa del constructor)
	SDL_RenderCopy(game->renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture); //en heap
}
