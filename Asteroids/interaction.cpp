#include "interaction.h"
#include <math.h>
#include <algorithm>

#define ACCELERATION 1.0f
#define MAX_SPEED 5.0f
#define PI 3.14159265358979323846
// Funzione per gestire il movimento della navicella
void moveSpaceship()
{
	spaceship->setXShiftValue(spaceship->getXShiftValue() + spaceship->getDx());
	spaceship->setYShiftValue(spaceship->getYShiftValue() + spaceship->getDy());
	wrapEntity(spaceship);
}
/**
 * This function handles keyboard input for controlling the spaceship.
 *
 * The function first calculates the current speed of the spaceship.
 * Then, depending on the key that was pressed, it performs the following actions:
 *
 * 'w': Accelerates the spaceship in the direction it is currently facing. If this would result in a speed greater than MAX_SPEED, the speed is capped at MAX_SPEED.
 * 's': Decelerates the spaceship. If this would result in a speed less than 0, the speed is set to 0.
 * ' ': Causes the spaceship to shoot.
 *
 * Any other key is ignored.
 */
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
// Funzione per gestire il movimento del mouse
void mouseMovement(int x, int y)
{
	float dx = x - spaceship->getXShiftValue();
	float dy = (height - y) - spaceship->getYShiftValue();
	float angle = atan2(-dx, dy) * 180 / PI;
	spaceship->setAngle(angle);
}
/**
 * This function checks if the given entity has collided with any of the asteroids.
 *
 * The function iterates over all asteroids and checks if the hitbox of the entity overlaps with the hitbox of the asteroid.
 * If a collision is detected, the function performs the following actions:
 *
 * If the asteroid is not a split asteroid, it is split into smaller asteroids.
 * The points of the asteroid are added to the spaceship's score.
 * The asteroid is removed from the list of asteroids.
 *
 * The function returns true if a collision was detected and false otherwise.
 */
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
/**
 * This function wraps the position of the entity around the screen.
 *
 * The function first calculates the new position of the entity by adding its current shift values to its current position.
 * Then it checks if the entity has moved past the right, left, top, or bottom edge of the screen.
 * If the entity has moved past an edge, its position is wrapped around to the opposite edge.
 */
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