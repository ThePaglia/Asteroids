#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
using namespace glm;
using namespace std;

extern const int width;
extern const int height;

typedef struct
{
	vec3 cornerBot;
	vec3 cornerTop;
} Hitbox;

class Entity;
class Spaceship;
class Projectile;
class Asteroid;
// Classe che rappresenta un'entità all'interno del gioco
class Entity
{
public:
	Hitbox hitbox;
	GLuint VAO;
	GLuint VBO_V;
	GLuint VBO_C;
	vector<vec3> vertices;
	vector<vec4> colors;
	mat4 model;				  // Matrice di modellazione: traslazione * rotazione * scala
	float xShiftValue;		  // Valore di traslazione per la x
	float yShiftValue;		  // Valore di traslazione per la y
	float xScaleValue;		  // Valore di scalatura per la x
	float yScaleValue;		  // Valore di scalatura per la y
	float dx;
	float dy;
	float angle;			  // Valore di rotazione
	bool backgroundComponent; // true se fa parte dello sfondo, false altrimenti
	bool alive;				  // true se l'entit� � viva, false altrimenti
	int points;

public:
	Entity();
	virtual ~Entity() {}
	void createPolygonalShape(vector<vec3> polygonVertices, vec4 color1, vec4 color2);
	void createHermiteShape(vector<vec3> controlPoints, vec3 center, vec4 color1, vec4 color2);
	void initVAO();
	void updateVAO();
	GLuint* getVAO();
	GLuint* getVerticesVBO();
	GLuint* getColorsVBO();
	vector<vec3>* getVertices();
	vector<vec4>* getVerticesColors();
	int getNumberOfVertices();
	mat4* getModel();
	float getXShiftValue();
	float getYShiftValue();
	float getXScaleValue();
	float getYScaleValue();
	float getAngle();
	void setXShiftValue(float value);
	void setYShiftValue(float value);
	void setXScaleValue(float value);
	void setYScaleValue(float value);
	float getDx();
	float getDy();
	void setDx(float value);
	void setDy(float value);
	void setAngle(float value);
	Hitbox getHitbox();
	float getWidth();
	float getHeight();
	Hitbox getHitboxWorldCoordinates();
	bool isBackgroundComponent();
	void changePane();
	void die();
	bool isAlive();
	void updatePosition();
	int getPoints();
	void addPoints(int value);
};
// Classe che rappresenta la navicella spaziale
class Spaceship : public Entity
{
private:
	float size;
	int points;
public:
	Spaceship();
	float getSize();
	void setSize(float value);
	void shoot();
	void rotateHitbox();
};
// Classe che rappresenta un proiettile
class Projectile : public Entity
{
public:
	Projectile(float x, float y, float angle);
	bool isInScreen();
	void changeStatus();
};
// Classe che rappresenta un asteroide
class Asteroid : public Entity
{
private:
	bool isSplit;
public:
	Asteroid();
	int getPoints();
	void setPoints(int value);
	void split();
	bool getIsSplit();
};