#include "entity.h"
#include "VAO_manager.h"
#include "hermite.h"
#include "hermiteFromFile.h"
#include <math.h>

#define DEFAULT_SIZE 25.0f
#define DEFAULT_PROJECTILE_SPEED 10.0f
#define P_VAL 100

Entity::Entity()
{
	backgroundComponent = false;
	xShiftValue = 0.0f;
	yShiftValue = 0.0f;
	xScaleValue = DEFAULT_SIZE;
	yScaleValue = DEFAULT_SIZE;
	dx = 0.0f;
	dy = 0.0f;
	angle = 0.0f;
}

void Entity::createPolygonalShape(vector<vec3> polygonVertices, vec4 color1, vec4 color2)
{
	vertices.push_back(vec3(0.0f, 0.0f, 0.0f));
	colors.push_back(color2);
	float xMin = polygonVertices[0].x;
	float yMin = polygonVertices[0].y;
	float xMax = polygonVertices[0].x;
	float yMax = polygonVertices[0].y;
	for (vec3 vertex : polygonVertices)
	{
		vertices.push_back(vec3(vertex.x, vertex.y, 0.0f));
		colors.push_back(color1);
		if (vertex.x < xMin)
			xMin = vertex.x;
		else if (vertex.x > xMax)
			xMax = vertex.x;
		if (vertex.y < yMin)
			yMin = vertex.y;
		else if (vertex.y > yMax)
			yMax = vertex.y;
	}
	hitbox.cornerBot = vec3(xMin, yMin, 0.0f);
	hitbox.cornerTop = vec3(xMax, yMax, 0.0f);
}

void Entity::createHermiteShape(vector<vec3> controlPoints, vec3 center, vec4 color1, vec4 color2)
{
	Shape derivative;
	Shape polygonal;
	polygonal.cpCoordinates = controlPoints;
	float* t = new float[polygonal.cpCoordinates.size()];
	for (int i = 0; i < polygonal.cpCoordinates.size(); i++)
		t[i] = (float)i / (float)(polygonal.cpCoordinates.size() - 1);

	float p_t = 0, p_b = 0, p_c = 0;
	float passotg = 1.0f / (float)(P_VAL - 1);

	float tgmapp, ampiezza;
	int is = 0;

	vertices.push_back(center);
	colors.push_back(color2);

	for (int i = 0; i < controlPoints.size(); i++)
		derivative.cpCoordinates.push_back(vec3(0.0f, 0.0f, 0.0f));

	for (float tg = 0.0f; tg <= 1.0f; tg += passotg)
	{
		if (tg > t[is + 1])
			is++;
		ampiezza = (t[is + 1] - t[is]);
		tgmapp = (tg - t[is]) / ampiezza;

		float x = controlPoints[is].x * PHI0(tgmapp) + DX(is, t, &derivative, &polygonal) * PHI1(tgmapp) * ampiezza + controlPoints[is + 1].x * PSI0(tgmapp) + DX(is + 1, t, &derivative, &polygonal) * PSI1(tgmapp) * ampiezza;
		float y = controlPoints[is].y * PHI0(tgmapp) + DY(is, t, &derivative, &polygonal) * PHI1(tgmapp) * ampiezza + controlPoints[is + 1].y * PSI0(tgmapp) + DY(is + 1, t, &derivative, &polygonal) * PSI1(tgmapp) * ampiezza;

		vertices.push_back(vec3(x, y, 0.0f));
		colors.push_back(color1);
	}
	vertices.push_back(controlPoints[controlPoints.size() - 1]);
	colors.push_back(color1);
	float xMin = vertices[0].x;
	float yMin = vertices[0].y;
	float xMax = vertices[0].x;
	float yMax = vertices[0].y;
	for (vec3 vertex : vertices)
	{
		if (vertex.x < xMin)
			xMin = vertex.x;
		else if (vertex.x > xMax)
			xMax = vertex.x;
		if (vertex.y < yMin)
			yMin = vertex.y;
		else if (vertex.y > yMax)
			yMax = vertex.y;
	}
	hitbox.cornerBot = vec3(xMin, yMin, 0.0f);
	hitbox.cornerTop = vec3(xMax, yMax, 0.0f);
}

void Entity::initVAO()
{
	initShapeVAO(this);
}

void Entity::updateVAO()
{
	updateShapeVAO(this);
}

GLuint* Entity::getVAO()
{
	return &VAO;
}

GLuint* Entity::getVerticesVBO()
{
	return &VBO_V;
}

GLuint* Entity::getColorsVBO()
{
	return &VBO_C;
}

vector<vec3>* Entity::getVertices()
{
	return &vertices;
}

vector<vec4>* Entity::getVerticesColors()
{
	return &colors;
}

int Entity::getNumberOfVertices()
{
	return vertices.size();
}

mat4* Entity::getModel()
{
	return &model;
}

float Entity::getXShiftValue()
{
	return xShiftValue;
}

float Entity::getYShiftValue()
{
	return yShiftValue;
}

float Entity::getXScaleValue()
{
	return xScaleValue;
}

float Entity::getYScaleValue()
{
	return yScaleValue;
}

float Entity::getAngle()
{
	return angle;
}

void Entity::setXShiftValue(float value)
{
	xShiftValue = value;
}

void Entity::setYShiftValue(float value)
{
	yShiftValue = value;
}

void Entity::setXScaleValue(float value)
{
	xScaleValue = value;
}

void Entity::setYScaleValue(float value)
{
	yScaleValue = value;
}

void Entity::setAngle(float value)
{
	angle = value;
}

Hitbox Entity::getHitbox()
{
	return hitbox;
}

float Entity::getWidth()
{
	return hitbox.cornerTop.x - hitbox.cornerBot.x;
}

float Entity::getHeight()
{
	return hitbox.cornerTop.y - hitbox.cornerBot.y;
}

Hitbox Entity::getHitboxWorldCoordinates()
{
	float xBottom = (float)width / 2 + hitbox.cornerBot.x * xScaleValue + xShiftValue;
	float yBottom = (float)height / 2 + hitbox.cornerBot.y * yScaleValue + yShiftValue;
	float xTop = (float)width / 2 + hitbox.cornerTop.x * xScaleValue + xShiftValue;
	float yTop = (float)height / 2 + hitbox.cornerTop.y * yScaleValue + yShiftValue;
	return { vec3(xBottom, yBottom, 0.0f), vec3(xTop, yTop, 0.0f) };
}

bool Entity::isBackgroundComponent()
{
	return backgroundComponent;
}

void Entity::changePane()
{
	backgroundComponent = !backgroundComponent;
}

void Entity::die()
{
	alive = alive ? false : true;
}

bool Entity::isAlive()
{
	return alive;
}

Spaceship::Spaceship()
{
	alive = true;
	size = 1.0f;
	dy = 0.0f;
	dx = 0.0f;
}

void Entity::setDx(float value)
{
	dx = value;
}

void Entity::setDy(float value)
{
	dy = value;
}

float Entity::getDx()
{
	return dx;
}

float Entity::getDy()
{
	return dy;
}

void Entity::updatePosition()
{
	xShiftValue += dx;
	yShiftValue += dy;
	wrapEntity(this);
}

void Spaceship::rotateHitbox()
{
	vec4 t1 = vec4(hitbox.cornerBot.x, hitbox.cornerTop.y, 0.0f, 1.0f);
	vec4 b1 = vec4(hitbox.cornerTop.x, hitbox.cornerBot.y, 0.0f, 1.0f);
	mat4 model = mat4(1.0);
	model = rotate(model, radians(-90.0f), vec3(0.0f, 0.0f, 1.0f));
	hitbox.cornerTop = model * t1;
	hitbox.cornerBot = model * b1;
}

float Spaceship::getSize()
{
	return size;
}

void Spaceship::setSize(float value)
{
	size = value;
}

extern vector<Entity*> projectiles;

void Spaceship::shoot()
{
	float x = spaceship->getXShiftValue();
	float y = spaceship->getYShiftValue();
	Projectile* projectile = new Projectile(x, y, spaceship->getAngle());
	projectile->initVAO();
	projectiles.push_back(projectile);
}

Projectile::Projectile(float x, float y, float angle)
{
	this->angle = angle;
	xShiftValue = x;
	yShiftValue = y;
	dx = DEFAULT_PROJECTILE_SPEED * cos(radians(90.0f + angle));
	dy = DEFAULT_PROJECTILE_SPEED * sin(radians(90.0f + angle));
	createHermiteShape(readPolygonVertices((char*)"projectile.txt"), vec3(0.0f, 0.0f, 0.0f), vec4(1.0f, 1.0f, 0.0f, 1.0f), vec4(1.0f, 1.0f, 0.0f, 1.0f));
	xScaleValue = (float)DEFAULT_SIZE * 2 / 3;
	yScaleValue = (float)DEFAULT_SIZE * 2 / 3;
}

bool Projectile::isInScreen()
{
	return xShiftValue > 0.0f && xShiftValue < (float)width && yShiftValue > 0.0f && yShiftValue < (float)height;
}

Asteroid::Asteroid()
{
	alive = true;
	dx = ((float)(rand() % 5 - 1));
	dy = ((float)(rand() % 5 - 1));
	xScaleValue = DEFAULT_SIZE * 3;
	yScaleValue = DEFAULT_SIZE * 3;
	isSplit = false;
}

void Asteroid::split()
{
	vec4 color1 = vec4(0.7f, 0.0f, 0.7f, 1.0f);
	vec4 color2 = vec4(0.7f, 0.7f, 0.7f, 1.0f);
	char* controlPointsFile = (char*)"asteroid.txt";
	vector<vec3> controlPoints = readPolygonVertices(controlPointsFile);

	// Creare due nuovi asteroidi con dimensioni ridotte
	Asteroid* asteroid1 = new Asteroid();
	asteroid1->createHermiteShape(controlPoints, vec3(0.0f, 0.0f, 0.0f), color1, color2);
	asteroid1->xScaleValue = DEFAULT_SIZE * 3 / 2;
	asteroid1->yScaleValue = DEFAULT_SIZE * 3 / 2;
	asteroid1->isSplit = true;

	Asteroid* asteroid2 = new Asteroid();
	asteroid2->createHermiteShape(controlPoints, vec3(0.0f, 0.0f, 0.0f), color1, color2);
	asteroid2->xScaleValue = DEFAULT_SIZE * 3 / 2;
	asteroid2->yScaleValue = DEFAULT_SIZE * 3 / 2;
	asteroid2->isSplit = true;

	// Impostare le posizioni dei nuovi asteroidi sulla posizione dell'asteroide corrente
	asteroid1->xShiftValue = this->xShiftValue;
	asteroid1->yShiftValue = this->yShiftValue;
	asteroid2->xShiftValue = this->xShiftValue;
	asteroid2->yShiftValue = this->yShiftValue;

	// Impostare direzioni casuali per i nuovi asteroidi
	asteroid1->dx = ((float)(rand() % 6 + 1));
	asteroid1->dy = ((float)(rand() % 6 + 1));
	asteroid2->dx = ((float)(rand() % 6 + 1));
	asteroid2->dy = ((float)(rand() % 6 + 1));

	// Creare la forma degli asteroidi
	asteroid1->initVAO();
	asteroid2->initVAO();

	// Aggiungere i nuovi asteroidi all'array asteroids
	asteroids.push_back(asteroid1);
	asteroids.push_back(asteroid2);
}

bool Asteroid::getIsSplit()
{
	return isSplit;
}

void Projectile::changeStatus()
{
	alive = alive ? false : true;
}