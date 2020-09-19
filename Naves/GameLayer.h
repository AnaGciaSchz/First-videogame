#pragma once
#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"

#include <list>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	virtual void processControls() override; //procesar controles introducidos por el usuario
	virtual void update() override; //actualizar
	virtual void draw() override; //dibujar
	void keysToControls(SDL_Event event);

	Player* player;
	Background* background;
	//Necesario inicializar aquí, no nos interesa que tenga otros valores de inicio
	bool controlShoot = false; //para disparar
	int controlMoveY = 0;
	int controlMoveX = 0;
	int newEnemyTime = 0; //Cada cuanto aparece un enemigo nuevo
	int killedEnemys = 0; //número de enemigos matados para ir incrementando la dificultad del juego

	list<Enemy*> enemies; //lista enlazada, arraylist sería vector, array es un array con cosillas
	list<Projectile*> projectiles;
};

