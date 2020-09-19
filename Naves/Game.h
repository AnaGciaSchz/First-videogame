#pragma once

// Todas las librerias de SDL y otras utilidades
#include <iostream>
#include <string>
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
	bool scaledToMax = false;
	float scaleLower = 1;
	SDL_Window* window; // ventana
	SDL_Renderer* renderer; // renderizador
	bool loopActive; // Juego activo

	Layer* gameLayer;
};


