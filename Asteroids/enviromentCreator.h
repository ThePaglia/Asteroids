#pragma once
#include "entity.h"
#include "hermiteFromFile.h"

extern const int width;
extern const int height;

vector<Entity*> createAsteroids(char* controlPointsFile);

vector<Entity*> createLives(float numberOfLives, float radius);

vector<Entity*> createSpaceship(Spaceship* spaceship);