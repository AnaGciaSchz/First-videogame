#pragma once

#include "Game.h"

class Animation
{
public:
	Animation(string filename, float actorWidth, float actorHeight,
		float fileWidth, float fileHeight,
		int updateFrecuence, int totalFrames, Game* game);
	bool update();
	void draw(float x, float y);
	int actorWidth; // Actor Width
	int actorHeight; // Actor height
	int fileWidth; // Sprite sheet width
	int fileHeight; // Sprite sheet height
	int currentFrame;
	int totalFrames;
	Game* game;

	// Auxiliares
	SDL_Texture* texture; // imagen
	SDL_Rect source; // Parte de la imagen que se recorta del fichero
	float frameWidth; // Ancho de un fotograma
	float frameHeigt; // Alto de un fotograma
	int updateFrecuence = 0;
	int updateTime = 0; //cuantas iteraciones hace que se cambi� de frame
};

