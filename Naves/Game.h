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
	bool scaledToMax = false;
	float scaleLower = 1;
	SDL_Texture* getTexture(string filename);
	map<string, SDL_Texture*> mapTextures; // map - cache
	SDL_Window* window; // ventana
	SDL_Renderer* renderer; // renderizador
	TTF_Font* font;//Fuente para escribir, sirve cualquier fuente TTF, nos sirve porque ya tenemos el #include <SDL_ttf.h>)
	bool loopActive; // Juego activo

	Layer* gameLayer;

	int const stateMoving = 1;
	int const stateJumping = 2;
	int const stateDying = 3;
	int const stateDead = 4;
	int const stateShooting = 5;

	int const orientationRight = 1;
	int const orientationLeft = 2;

	int currentLevel = 0;
	int finalLevel = 2;

	int input;
	int const inputKeyboard = 1;
	int const inputMouse = 2;
	int const inputGamePad = 3;

};


