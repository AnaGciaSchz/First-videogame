#pragma once
#include "Actor.h"
#include "Animation.h" 

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void update();
	void impacted(); // Recibe impacto y pone animación de morir
	float vxIntelligence;
	int state;
	void draw(float scrollX = 0) override; // Va a sobrescribir
	Animation* aDying;
	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada

};

