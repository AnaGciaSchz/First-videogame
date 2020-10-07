#pragma once
#include "Actor.h"
#include "Player.h"

class Coin : public Actor
{
public:
	Coin(float x, float y, Game* game);
	void update();
	void addLife(Player * player);

};