#pragma once
#include "Player.h"
#include "Enemy.h"

#include <list>

class Bomb : public Actor
{
public:
	Bomb(float x, float y, Game* game);
	void update();
	void removeAllEnemies(list<Enemy*> enemies, list<Enemy*>* deleteEnemies);

};