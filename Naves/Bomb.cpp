#include "Bomb.h"

Bomb::Bomb(float x, float y, Game* game)
	: Actor("res/bomb.png", x, y, 47, 48, game) {

	vx = -1;
}

void Bomb::update() {
	x = x + vx;
}

void Bomb::removeAllEnemies(list<Enemy*> enemies, list<Enemy*>* deleteEnemies)
{
	for (auto const& enemy : enemies) {
		bool eInList = std::find(deleteEnemies->begin(),
			deleteEnemies->end(),
			enemy) != deleteEnemies->end();

		if (!eInList) {
			deleteEnemies->push_back(enemy);
		}
	}
}

