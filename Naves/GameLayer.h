#pragma once
#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Tile.h"

#include <list>
#include <fstream>
#include <sstream>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	virtual void processControls() override; //procesar controles introducidos por el usuario
	virtual void update() override; //actualizar
	virtual void draw() override; //dibujar
	void keysToControls(SDL_Event event);

	void loadMap(string name);
	void loadMapObject(char character, float x, float y);

	int mapWidth;
	list<Tile*> tiles;

	Text* textPoints; //texto de los puntos
	int points; //cuantos puntos tenemos
	Player* player;
	Background* background;
	Actor* backgroundPoints; //Icono para los puntos
	//Necesario inicializar aqu�, no nos interesa que tenga otros valores de inicio
	bool controlShoot = false; //para disparar
	int controlMoveY = 0;
	int controlMoveX = 0;
	int killedEnemys = 0; //n�mero de enemigos matados para ir incrementando la dificultad del juego

	list<Enemy*> enemies; //lista enlazada, arraylist ser�a vector, array es un array con cosillas
	list<Projectile*> projectiles;
};

