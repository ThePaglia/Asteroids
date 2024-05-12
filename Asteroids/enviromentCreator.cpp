#include "enviromentCreator.h"

#define MAX_SPEED 3
#define NUMBER_OF_ASTEROIDS 5

vector<Entity*> createAsteroids(char* controlPointsFile)
{
	vector<Entity*> asteroids;
	vec4 color1 = vec4(0.5f, 0.0f, 0.5f, 1.0f);
	vec4 color2 = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	vector<vec3> controlPoints = readPolygonVertices(controlPointsFile);
	for (int i = 0; i < NUMBER_OF_ASTEROIDS; i++)
	{
		Asteroid* asteroid = new Asteroid();
		asteroid->createHermiteShape(controlPoints, vec3(0.0f, 0.0f, 0.0f), color1, color2);
		// Randomize the asteroid's position on the edge of the screen
		float x = (float)(rand() % width);
		float y = (float)(rand() % height);
		asteroid->setXShiftValue(x);
		asteroid->setYShiftValue(y);
		// Randomize the asteroid's speed
		asteroid->setDx((float)(rand() % MAX_SPEED + 1));
		asteroid->setDy((float)(rand() % MAX_SPEED + 1));
		asteroids.push_back(asteroid);
	}
	return asteroids;
}

// Crea il player che è un triangolo equilatero
vector<Entity*> createSpaceship(Spaceship* spaceship)
{
	vector<Entity*> spaceshipVector;
	vec4 color1 = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	vec4 color2 = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	spaceship->createPolygonalShape(createTriangle(1.5f), color1, color2);
	spaceship->setXShiftValue((float)width / 2);
	spaceship->setYShiftValue((float)height / 2);
	spaceshipVector.push_back(spaceship);
	return spaceshipVector;
}

vector<Entity*> createLives(float numberOfLives, float radius)
{
	vec4 red = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vec4 pink = vec4(0.5f, 0.2f, 0.2f, 1.0f);
	vector<Entity*> lives;
	for (int i = 0; i < numberOfLives; i++)
		lives.push_back(new Entity());
	int i = 0;
	for (Entity* heart : lives)
	{
		heart->createPolygonalShape(createHeart(radius, radius, 100), red, pink);
		heart->setYShiftValue((float)height - heart->getHeight() / 2 * heart->getYScaleValue());
		heart->setXShiftValue(5.0f + heart->getWidth() / 2 * heart->getXScaleValue() + i * (heart->getWidth() * heart->getXScaleValue() + 5.0f));
		i++;
	}
	return lives;
}