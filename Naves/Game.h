#pragma once

// Todas las librerias de SDL y otras utilidades
#include <iostream>
#include <string>
#include <map> 
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include"Layer.h"
class Layer; //quitar dependencia cíclica

// Valores generales
#define WIDTH 480 //ancho de la pantalla
#define HEIGHT 320 //alto de la pantalla

class Game
{
public:
	Game();
	void loop();
	void scale();
	SDL_Texture* getTexture(string filename);
	map<string, SDL_Texture*> mapTextures; // map - cache
	bool scaledToMax = false;
	float scaleLower = 1;
	SDL_Window* window; // ventana
	SDL_Renderer* renderer; // renderizador
	TTF_Font* font;//Fuente para escribir, sirve cualquier fuente TTF, nos sirve porque ya tenemos el #include <SDL_ttf.h>)
	bool loopActive; // Juego activo

	Layer* gameLayer;
};


