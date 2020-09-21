#pragma once

#include "Actor.h"


class Recolectable : public Actor
{

public:
	Recolectable(float x, float y, Game* game);
	void update();
};

