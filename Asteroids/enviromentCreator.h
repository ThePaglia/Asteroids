#pragma once
#include "entity.h"
#include "hermiteFromFile.h"

extern const int width;
extern const int height;
// Create a vector of entities that represent the asteroids
vector<Entity*> createAsteroids(char* controlPointsFile);
// Create a vector of entities that represent the bullets
vector<Entity*> createLives(float numberOfLives, float radius);
// Create a vector of entities that represent the bullets
vector<Entity*> createSpaceship(Spaceship* spaceship);