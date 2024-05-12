#include "interaction.h"
#include <math.h>
#include <algorithm>

#define ACCELERATION 1.0f
#define MAX_SPEED 5.0f
#define PI 3.14159265358979323846

void moveSpaceship()
{
	spaceship->setXShiftValue(spaceship->getXShiftValue() + spaceship->getDx());
	spaceship->setYShiftValue(spaceship->getYShiftValue() + spaceship->getDy());
	wrapEntity(spaceship);
}

void keyboard(unsigned char key, int x, int y)
{
	float magnitude = sqrt(spaceship->getDx() * spaceship->getDx() + spaceship->getDy() * spaceship->getDy());
	switch (key)
	{
	case 'w':
		spaceship->setDx(spaceship->getDx() - ACCELERATION * sin(spaceship->getAngle() * PI / 180));
		spaceship->setDy(spaceship->getDy() - ACCELERATION * -cos(spaceship->getAngle() * PI / 180));
		if (magnitude > MAX_SPEED)
		{
			spaceship->setDx(spaceship->getDx() / magnitude * MAX_SPEED);
			spaceship->setDy(spaceship->getDy() / magnitude * MAX_SPEED);
		}
		break;
	case 's':
		if (magnitude > 0)
		{
			float newMagnitude = std::max(0.0f, magnitude - ACCELERATION);
			spaceship->setDx(spaceship->getDx() / magnitude * newMagnitude);
			spaceship->setDy(spaceship->getDy() / magnitude * newMagnitude);
		}
		break;
	case ' ':
		spaceship->shoot();
		break;
	default:
		break;
	}
}

void mouseMovement(int x, int y)
{
	float dx = x - spaceship->getXShiftValue();
	float dy = (height - y) - spaceship->getYShiftValue();
	float angle = atan2(-dx, dy) * 180 / PI;
	spaceship->setAngle(angle);
}

bool checkEnemyCollision(Entity* entity)
{
	for (int i = 0; i < asteroids.size(); i++)
	{
		if (entity->getHitboxWorldCoordinates().cornerBot.x <= asteroids[i]->getHitboxWorldCoordinates().cornerTop.x
			&& entity->getHitboxWorldCoordinates().cornerTop.x >= asteroids[i]->getHitboxWorldCoordinates().cornerBot.x
			&& entity->getHitboxWorldCoordinates().cornerBot.y <= asteroids[i]->getHitboxWorldCoordinates().cornerTop.y
			&& entity->getHitboxWorldCoordinates().cornerTop.y >= asteroids[i]->getHitboxWorldCoordinates().cornerBot.y)
		{
			Asteroid* asteroid = dynamic_cast<Asteroid*>(asteroids[i]);
			if (asteroid && !asteroid->getIsSplit()) {
				asteroid->split();
			}
			spaceship->addPoints(asteroids[i]->getPoints());
			delete(asteroids[i]);
			asteroids.erase(asteroids.begin() + i);
			i--;
			return true;
		}
	}
	return false;
}

void wrapEntity(Entity* entity)
{
	float x = entity->getXShiftValue() + entity->getDx();
	float y = entity->getYShiftValue() + entity->getDy();

	// Controlla se l'entità ha superato il bordo destro dello schermo
	if (x > width)
	{
		x = 0.0f;
	}
	// Controlla se l'entità ha superato il bordo sinistro dello schermo
	else if (x < 0.0f)
	{
		x = width;
	}

	// Controlla se l'entità ha superato il bordo superiore dello schermo
	if (y > height)
	{
		y = 0.0f;
	}
	// Controlla se l'entità ha superato il bordo inferiore dello schermo
	else if (y < 0.0f)
	{
		y = height;
	}

	entity->setXShiftValue(x);
	entity->setYShiftValue(y);
}