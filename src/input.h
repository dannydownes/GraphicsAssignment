#pragma once
#include "SDL.h"
#include <iostream>
#include "load.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp> // NOTE: must do before including GLM headers // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 


class Input
{
public:
	//constructor
	Input();

	//public functions
	void Action(bool *run, Load &load);
	void ActionUpdate(Load &load, bool &run);
	bool up, down, left, right = false;
	float shotX;
	// set 0.005
	glm::vec2 bulletTop;
	int allDead = 36;
	char sNum[3], lNum[3];
	float enemySpeed = 0.002f;
	float enemyDrop = 0.025f;
	bool enemyLeft = false;
	bool enemyRight = true;

	float newAR;

private:
	bool fullscreen = false;
	bool fire = false;
};
