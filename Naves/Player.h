#pragma once

#include "Actor.h"
#include "Projectile.h" 
#include "Audio.h"

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	Projectile* shoot();
	Audio* audioShoot;
	int playerLifes = 3;

	void loseLife();
	bool isDead();
	void update();
	void moveX(float axis);
	void moveY(float axis);
	int shootCadence = 30;
	int shootTime = 0;

};

