#pragma once
#include <GL/glew.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp> // NOTE: must do before including GLM headers // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

class Load
{
public:
	// CONSTRUCTOR
	Load();

	// FUNCTIONS
	void CreateWindow();
	void CreateOpenGL();
	void Initialise();
	void InitGL();
	void Render();
	void Text(char* mess, GLuint &texture, SDL_Surface* surf);

	// WINDOW
	SDL_DisplayMode screenSize;
	int winW, winH;
	SDL_Window *win = nullptr;

	// OPENGL
	SDL_GLContext glcontext = nullptr;
	GLint success;
	GLuint shaderProgram;
	GLuint VBO, EBO, VAO;
	float aspectRatio;

	// GL TEXTURES
	GLuint tEnemy, tPlayer, tPlayerBullet, tBackground, tScore, tLives, tFrame, tShieldFull, tShieldMid, tShieldLast, tEnemyBullet, tExplosion;

	//PLAYER STUFF
	float playerX = 2.0f;
	float bulletX, bulletY;
	int counter = 0;

	// ENEMY STUFF
	bool enemyInit;
	glm::vec2 enemyPos[36];
	bool enemyAlive[36];
	glm::vec2 enemyBulletPos = {10.0f, 10.0f};
	float bulletControl = 60.0f;

	// BARRIER STUFF
	glm::vec2 shield[3] = {{0.80f,0.75f},{1.80f ,0.75f},{2.80f,0.75f}};
	float health[3] = {6.0f, 6.0f, 6.0f};

	//Explosion
	glm::vec2 explosionPos = { 10.0f, 10.0f };
	float eTimer;
	float eHide = 10.0f;

	// CONFUSING STUFF
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	GLint modelLocation, viewLocation, projectionLocation;
	glm::mat4 _transRotate;
	glm::mat4 _transTranslate;
	glm::mat4 _transScale;

	// TEXT STUFF
	SDL_Color textColor = { 0, 0, 0 };
	int scoreNum = 0;
	int livesNum = 3;
	char score[30] = {"Score: "};
	char lives[30] = {"Lives: "};
	float angle = 0.0f;
	SDL_Surface *scoreS, *livesS;

	// DESTRUCTOR
	~Load();
private:
	
};