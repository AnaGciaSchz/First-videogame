#pragma once
#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Audio.h" 
#include "Tile.h"
#include "Space.h" // importar

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
	Audio* audioBackground;

	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	void calculateScroll();
	float scrollX;
	Tile* cup; // Elemento de final de nivel

	int mapWidth;
	list<Tile*> tiles;
	Space* space;

	Text* textPoints; //texto de los puntos
	int points; //cuantos puntos tenemos
	Player* player;
	Background* background;
	Actor* backgroundPoints; //Icono para los puntos
	//Necesario inicializar aquí, no nos interesa que tenga otros valores de inicio
	bool controlShoot = false; //para disparar
	int controlMoveY = 0;
	int controlMoveX = 0;
	int killedEnemys = 0; //número de enemigos matados para ir incrementando la dificultad del juego

	// Elementos de interfaz
	Actor* buttonJump;
	Actor* buttonShoot;

	list<Enemy*> enemies; //lista enlazada, arraylist sería vector, array es un array con cosillas
	list<Projectile*> projectiles;
};

