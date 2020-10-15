#pragma once
#include "Game.h" //porque tiene el render

class Actor
{
public:
	Actor(string filename, float x, float y, int width, int height, Game* game); //game es quien lo pinta
	~Actor();
	virtual void draw(float scrollX = 0);
	bool isInRender(float scrollX = 0);
	bool isOverlap(Actor* actor);

	SDL_Texture* texture; //cosas de SDL, no importante
	int x; //pos, como no se inicializa, su valor es aleatorio (lo que haya en ese momento en memoria
	int y; //pos
	float vx; //velocidad en x
	float vy; //velocidad y
	int width;
	int height;
	int fileWidth; //tamaño imagen
	int fileHeight; //tamaño imagen
	bool collisionDown;
	bool outRight;
	bool outLeft;
	Game* game;

	bool containsPoint(int pointX, int pointY); // contiene punto
	bool clicked; // Está pulsado
};

