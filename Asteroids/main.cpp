#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>
#include "lib.h"
#include "interaction.h"

const int width = 1280;
const int height = 720;

static unsigned int programID, programID_text;

GLuint textVAO, textVBO;
GLuint isBackgroundShader, resolution, finalColor, currentFrame;

mat4 Projection;
GLuint MatProj, MatModel;

vector<Entity*> projectiles;
vector<Entity*> asteroids;
vector<vector<Entity*>*> scene;
vector<Entity*> lives;
vector<Entity*> background;
vector<Entity*> spaceshipVector;

Spaceship* spaceship = new Spaceship();

void INIT_SHADER(void)
{
	GLenum ErrorCheckValue = glGetError();
	char* vertexShader = (char*)"vertexShader_M.glsl";
	char* fragmentShader = (char*)"fragmentShader_M.glsl";
	programID = ShaderMaker::createProgram(vertexShader, fragmentShader);
	glUseProgram(programID);

	//Generazione del program shader per la gestione del testo
	vertexShader = (char*)"vertexShader_Text.glsl";
	fragmentShader = (char*)"fragmentShader_Text.glsl";

	programID_text = ShaderMaker::createProgram(vertexShader, fragmentShader);
}
// Inizializzazione dei VAO per la scena
void INIT_VAO(void)
{
	spaceshipVector = createSpaceship(spaceship);
	asteroids = createAsteroids((char*)"asteroid.txt");
	lives = createLives(3, 0.05f);

	Entity* backgroundPane = new Entity();
	backgroundPane->createPolygonalShape(createRectangle(2.0f, 2.0f), vec4(0.0f, 0.0f, 0.0f, 0.0f), vec4(0.0f, 0.0f, 0.0f, 0.0f));
	backgroundPane->setXScaleValue((float)width);
	backgroundPane->setYScaleValue((float)height);
	backgroundPane->setXShiftValue((float)width / 2);
	backgroundPane->setYShiftValue((float)height / 2);
	backgroundPane->changePane();

	background.push_back(backgroundPane);

	scene.push_back(&background);
	scene.push_back(&spaceshipVector);
	scene.push_back(&projectiles);
	scene.push_back(&asteroids);
	scene.push_back(&lives);

	for (vector<Entity*>* container : scene)
		for (Entity* entity : *container)
			entity->initVAO();

	Projection = ortho(0.0f, float(width), 0.0f, float(height));
	MatProj = glGetUniformLocation(programID, "Projection");
	MatModel = glGetUniformLocation(programID, "Model");
	isBackgroundShader = glGetUniformLocation(programID, "isBackgroundShader");
	resolution = glGetUniformLocation(programID, "resolution");
	finalColor = glGetUniformLocation(programID, "finalColor");
	currentFrame = glGetUniformLocation(programID, "time");

	glViewport(0, 0, width, height);
}

void shiftLeft(int index)
{
	for (int i = index; i < projectiles.size() - 1; i++)
		projectiles[i] = projectiles[i + 1];
	projectiles.pop_back();
}

void gameOver(char* text)
{
	string str(text);
	renderText(programID_text, Projection, str, textVAO, textVBO, width / 2 - str.length() * 10.0f, height / 2, 1.0f, vec3(1.0f, 0.0f, 0.0f));
}

void update(int value)
{
	int i = 0;

	moveSpaceship();

	for (Entity* projectile : projectiles)
	{
		Projectile* p = dynamic_cast<Projectile*>(projectile);
		if (p->isInScreen()) {
			p->updatePosition();
		}
		else {
			// Rimuove il proiettile dalla scena
			shiftLeft(i);
			i--;
		}
	}

	if (projectiles.size() > 0)
	{
		for (Entity* projectile : projectiles)
		{
			if (checkEnemyCollision(projectile))
			{
				shiftLeft(i);
				i--;
			}
			i++;
		}
	}

	if (checkEnemyCollision(spaceship)) {
		lives.pop_back();
		if (lives.size() > 0) {
			spaceship->setXShiftValue((float)width / 2);
			spaceship->setYShiftValue((float)height / 2);
			spaceship->setAngle(0.0f);
		}
		else {
			spaceship->die();
		}
	}

	for (Entity* asteroid : asteroids)
	{
		asteroid->updateVAO();
		asteroid->updatePosition();
	}

	for (vector<Entity*>* container : scene) {
		for (Entity* entity : *container) {
			entity->updateVAO();
		}
	}
	if (spaceship->isAlive() && asteroids.size() > 0) {
		glutTimerFunc(17, update, 0);
	}
	glutPostRedisplay();
}
// Funzione per disegnare la scena
void drawScene(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (vector<Entity*>* container : scene)
	{
		for (Entity* entity : *container)
		{
			*entity->getModel() = mat4(1.0);
			*entity->getModel() = translate(*entity->getModel(), vec3(entity->getXShiftValue(), entity->getYShiftValue(), 0.0f));
			*entity->getModel() = scale(*entity->getModel(), vec3(entity->getXScaleValue(), entity->getYScaleValue(), 1.0f));
			*entity->getModel() = rotate(*entity->getModel(), radians(entity->getAngle()), vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
			glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(*entity->getModel()));
			glUniform2f(resolution, width, height);

			if (entity->isBackgroundComponent()) {
				glUniform1i(isBackgroundShader, 1);
			}
			else {
				glUniform1i(isBackgroundShader, 0);
			}
			glUniform1f(currentFrame, glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
			glBindVertexArray(*entity->getVAO());
			glDrawArrays(GL_TRIANGLE_FAN, 0, entity->getNumberOfVertices());
			glBindVertexArray(0);
		}
	}
	if (spaceship->isAlive() && asteroids.size() == 0)
		gameOver((char*)"YOU WIN");
	else if (!spaceship->isAlive())
		gameOver((char*)"YOU LOSE");
	string str = "Asteroids left: " + to_string(asteroids.size());
	renderText(programID_text, Projection, str, textVAO, textVBO, width - str.length() * 10.0f, height - 20.0f, 0.5f, vec3(1.0f, 0.0f, 0.0f));
	str = "Points: " + to_string(spaceship->getPoints());
	renderText(programID_text, Projection, str, textVAO, textVBO, width / 2, height - 20.0f, 0.5f, vec3(1.0f, 0.0f, 0.0f));
	glutSwapBuffers();
	glUseProgram(programID);
}

int main(int argc, char* argv[])
{
	// Calcolo del random in base al clock del sistema
	srand(static_cast<unsigned int>(time(nullptr)));

	glutInit(&argc, argv);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Asteroid");
	glutDisplayFunc(drawScene);
	glewExperimental = GL_TRUE;
	glewInit();
	INIT_SHADER();
	INIT_VAO();
	initTextVAO(&textVAO, &textVBO);
	initFreetype();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutTimerFunc(17, update, 0);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouseMovement);
	glutMainLoop();
}