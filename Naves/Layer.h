#pragma once
#include "Game.h"
class Game;

class Layer
{
public:
	Layer(Game* game);
	//Por defecto vacíos
	virtual void init() {}; //iniciar la capa
	virtual void processControls() {}; //procesar controles introducidos por el usuario
	virtual void update() {}; //actualizar
	virtual void draw() {}; //dibujar

	Game* game;
};

