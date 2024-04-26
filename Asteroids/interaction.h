#pragma once
#include "entity.h"

extern Spaceship* spaceship;
extern vector<Entity*> asteroids;

extern const int width;
extern const int height;

void keyboard(unsigned char key, int x, int y);

void mouseMovement(int x, int y);

void moveSpaceship();

bool checkEnemyCollision(Entity* entity);

void wrapEntity(Entity* entity);