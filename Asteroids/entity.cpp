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

float Entity::getangle()
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

void Entity::setangle(float value)
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
	vec4 color1 = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 color2 = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	createPolygonalShape(createTriangle(size), color1, color2);
}

void Spaceship::updatePosition()
{
	// TODO: implementare movimento in base alla velocit� della spaceship
	// xShiftValue += xShift;
	// yShiftValue += yShift;
}

float Spaceship::getSize()
{
	return size;
}

float Spaceship::setSize(float value)
{
	size = value;
}

vector<Projectile*> Spaceship::getProjectiles()
{
	return projectiles;
}

void Spaceship::shoot()
{
	float x = xShiftValue + cos(angle) + xScaleValue;
	float y = yShiftValue + size + sin(angle) + yScaleValue;
	// TODO: terzo parametro da determinare in base alla posizione del mouse
	Projectile* projectile = new Projectile(x, y, );
	projectiles.push_back(projectile);
}

void Spaceship::removeProjectile(int index)
{
	for (int i = index; i < projectiles.size() - 1; i++)
		projectiles[i] = projectiles[i + 1];
	projectiles.pop_back();
}

Projectile::Projectile(float x, float y, float angle)
{
	m = tan(radians(90.0f + angle));
	angle = angle;
	xShiftValue = x;
	yShiftValue = y;
	xShift = DEFAULT_PROJECTILE_SPEED * cos(radians(90.0f + angle));
	yShift = m * xShift;
	float greyScale = 0.3f;
	createHermiteShape(readPolygonVertices((char*)"projectile.txt"), vec3(0.0f, 0.0f, 0.0f), vec4(greyScale, greyScale, greyScale, 1.0f), vec4(greyScale, greyScale, greyScale, 1.0f));
	xScaleValue = (float)DEFAULT_SIZE * 2 / 3;
	yScaleValue = (float)DEFAULT_SIZE * 2 / 3;
}

void Projectile::updatePosition()
{
	xShiftValue += xShift;
	yShiftValue += yShift;
}

Asteroid::Asteroid()
{
	alive = true;
}