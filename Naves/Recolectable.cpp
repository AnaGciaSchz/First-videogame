#include "Recolectable.h"

Recolectable::Recolectable(float x, float y, Game* game)
	: Actor("res/moneda.png", x, y, 40, 40, game) {

	vx = -0.5;
}

void Recolectable::update() {
	x = x + vx;
}

