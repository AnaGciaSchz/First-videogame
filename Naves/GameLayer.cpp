#include "GameLayer.h"

GameLayer::GameLayer(Game* game) : Layer(game) { //llamamos al super
	init();
}

void GameLayer::init() {
	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();


	delete player; //antes de crear nuevo personaje, se borra anterior

	points = 0;
	textPoints = new Text("0", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points);

	players[0] = new Player("res/jugador.png",50, 50,50,57,30, 3, game);
	players[1] =new Player("res/jugador2.png",50, 50,50,57,10, 4, game);

	player = players[0]; // new = se crea en el heap y no se borra
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game); //Va a estar en el 85% de la x y el 5% de y

	playersLifeIcon = new Actor("res/corazon.png", WIDTH * 0.05, HEIGHT * 0.07, 44, 36, game);
	playersLifeText = new Text("0", (WIDTH * 0.05) + 44, HEIGHT * 0.06, game);
	playersLifeText->content = to_string(player->playerLifes);

	projectiles.clear(); // Vaciar por si reiniciamos el juego

	coins.clear();

	bombs.clear();

	enemies.clear(); // Vaciar por si reiniciamos el juego
	enemies.push_back(new Enemy(300, 50, game));
	enemies.push_back(new Enemy(300, 200, game)); //crea instancia de enemy, la copia y mete la copia, al final tenemos 2
	//emplace_back (Enemy...) ´con esto nos ahorramos las copias

}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) { //como no es un puntero, pasamos referencia
		keysToControls(event);
	}
	//procesar controles
	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			projectiles.push_back(newProjectile);
		}
	}
	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}
	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}


}

//Necesario diferenciar cuando se pulsa y
//levanta para cambiar valores y que no parezca que
//está siempre pulsada
void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_QUIT) {
		game->loopActive = false;
	}
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE: //Salir del juego
			game->loopActive = false;
			break;
		case SDLK_3: //Poner en pantalla completa
			game->scale();
			break;
		case SDLK_1: //Poner en pantalla completa
			players[0]->x = player->x;
			players[0]->y = player->y;
			player = players[0];
			break;
		case SDLK_2: //Poner en pantalla completa
			players[1]->x = player->x;
			players[1]->y = player->y;
			player = players[1];
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}


	}

}

void GameLayer::update() {
	background->update();
	// Generar enemigos
	newEnemyTime--;
	if (newEnemyTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new Enemy(rX, rY, game));
		newEnemyTime = std::min(200 - killedEnemys, 50); //Cada vez que se mata un enemigo, estos aparecen antes
	}

	//Generar monedas
	newCoinTime--;
	if (newCoinTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		coins.push_back(new Coin(rX, rY, game));
		newCoinTime = 300;
	}

	//Generar bombas
	newBombTime--;
	if (newBombTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		bombs.push_back(new Bomb(rX, rY, game));
		newBombTime = 1000;
	}


	player->update();
	for (auto const& coin : coins) { //auto es como var o dynamic, infiere tipo
		coin->update();
	}
	for (auto const& enemy : enemies) { 
		enemy->update();
	}

	for (auto const& bomb : bombs) { 
		bomb->update();
	}

	//Listas temporales para anotar qué queremos eliminar (y no borrar de las listas mientras las recorremos)
	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	list<Coin*> deleteCoins;
	list<Bomb*> deleteBombs;

	//Colisiones player - Coin
	for (auto const& coin : coins) {
		if (player->isOverlap(coin)) {
			coin->addLife(player);
			playersLifeText->content = to_string(player->playerLifes);

			bool eInList = std::find(deleteCoins.begin(),
				deleteCoins.end(),
				coin) != deleteCoins.end(); //comprobar si la moneda ya estaba en la lista

			if (!eInList) {
				deleteCoins.push_back(coin);
			}
		}
	}

	//Colisiones player - Bomb
	for (auto const& bomb : bombs) {
		if (player->isOverlap(bomb)) {
			bomb->removeAllEnemies(enemies, &deleteEnemies);

			bool eInList = std::find(deleteBombs.begin(),
				deleteBombs.end(),
				bomb) != deleteBombs.end(); //comprobar si la bomba ya estaba en la lista

			if (!eInList) {
				deleteBombs.push_back(bomb);
			}
		}
	}


	// Colisiones, Player - Enemy
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			player->loseLife();
			playersLifeText->content = to_string(player->playerLifes);
			if (player->isDead()) {
				init();
				return; // Cortar el for
			}
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end(); //comprobar si el enemigo ya estaba en la lista

			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
		}
	}

	// Colisiones , Enemy - Projectile


	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) { //Si le da un proyectil
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end(); //comprobar si el proyectil ya estaba en la lista

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
				enemy->loseLife();
				if (enemy->isDead()) {
					bool eInList = std::find(deleteEnemies.begin(),
						deleteEnemies.end(),
						enemy) != deleteEnemies.end(); //comprobar si el enemigo ya estaba en la lista

					if (!eInList) {
						deleteEnemies.push_back(enemy);
					}
					points++;
					textPoints->content = to_string(points);
				}//isDead

			}//overlap
			if (enemy->x + enemy->width / 2 <= 0) { //si el enemigo se sale de la pantalla
				bool eInList = std::find(deleteEnemies.begin(),
					deleteEnemies.end(),
					enemy) != deleteEnemies.end(); //comprobar si el enemigo ya estaba en la lista

				if (!eInList) {
					deleteEnemies.push_back(enemy);
				}

			}

			if (projectile->x - projectile->width / 2 >= WIDTH) { //si el proyectil se sale de la pantalla
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end(); //comprobar si el proyectil ya estaba en la lista

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
			}
		}


		for (auto const& projectile : projectiles) {
			if (projectile->isInRender() == false) {

				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
			}
		}

	}

	//Eliminamos los enemigos y proyectiles que han colisionado
	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delCoin : deleteCoins) {
		coins.remove(delCoin);
	}
	deleteCoins.clear();

	for (auto const& delBomb: deleteBombs) {
		bombs.remove(delBomb);
	}
	deleteBombs.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();



	for (auto const& projectile : projectiles) {
		projectile->update();
	}

	//std::cout << "update GameLayer" << std::endl;
}

void GameLayer::draw() {
	background->draw();
	for (auto const& projectile : projectiles) {
		projectile->draw();
	}
	player->draw();

	for (auto const& coin : coins) {
		coin->draw();
	}

	for (auto const& bomb : bombs) {
		bomb->draw();
	}

	for (auto const& enemy : enemies) {
		enemy->draw();
	}
	textPoints->draw();
	backgroundPoints->draw(); //Lo pintamos el último para que los enemigos no lo tapen tampoco

	playersLifeText->draw();
	playersLifeIcon->draw();

	SDL_RenderPresent(game->renderer); // Renderiza
}
