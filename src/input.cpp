#include "input.h"

Input::Input()
{
	fullscreen = false;
	
}

void Input::Action(bool *run, Load &load)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			//registers keyboard events from the user
		case SDL_QUIT:
			SDL_Log("User closed application.");
			*run = false;
			break;
		//case SDL_WINDOWEVENT:
		//	if (event.window.windowID == SDL_GetWindowID(load.win))
		//		switch (event.window.event)
		//		{
		////		//case SDL_WINDOWEVENT_MAXIMIZED:
		////		//	fullscreen = true;
		////		//	SDL_SetWindowFullscreen(load.win, SDL_WINDOW_FULLSCREEN_DESKTOP);
		////		//	glViewport(0, 0, load.screenSize.w, load.screenSize.h);
		////		//	SDL_Log("User entered fullscreen . . .");
		////		//	break;
		//		case SDL_WINDOWEVENT_RESIZED:
		//			SDL_GetWindowSize(load.win, &load.winW, &load.winH);
		//			
		//			newAR = (float)load.winW / (float)load.winH;
		//			SDL_Log("newAR %f", newAR);
		//			
		//			if (newAR < 1.0f)
		//			{
		//				glViewport(0, 0, (GLsizei)load.winW, (GLsizei)(load.winH * newAR));
		//			}
		//			else
		//			{
		//				glViewport(0, 0, (GLsizei)(load.winW * newAR), (GLsizei)load.winH);
		//			}
		//			load.projection = glm::ortho(0.0f, 4.0f, 0.0f, 3.0f, -1.0f, 100.0f);
		//			break;
		//		default:
		//			break;
		//		}
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_F11:
				if (fullscreen == false)
				{
					fullscreen = true;
					SDL_SetWindowFullscreen(load.win, SDL_WINDOW_FULLSCREEN_DESKTOP);
					glViewport(0, 0, load.screenSize.w, load.screenSize.h);
					SDL_Log("User entered fullscreen . . .");
				}
				else if (fullscreen == true)
				{
					fullscreen = false;
					SDL_SetWindowFullscreen(load.win, SDL_WINDOW_SHOWN);
					SDL_SetWindowSize(load.win, load.winW, load.winH);
					glViewport(0, 0, load.winW, load.winH);
					SDL_Log("User exited fullscreen . . .");
				}
				break;
			case SDLK_ESCAPE:
				SDL_Log("User closed application . . .");
				*run = false;
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				right = true;
				break;
			case SDLK_LEFT:
			case SDLK_a:
				left = true;
				break;
			case SDLK_SPACE:
				if (fire == false)
				{
					shotX = (load.playerX + (0.1055f / 2.0f));
				}
				fire = true;
				break;
			default:
				break;
			}
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_RIGHT:
			case SDLK_d:
				right = false;
				break;
			case SDLK_LEFT:
			case SDLK_a:
				left = false;
				break;
			default:
				break;
			}
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button)
			{
			case SDL_BUTTON_LEFT:
				if (fire == false)
				{
					shotX = (load.playerX + (0.1055f / 2.0f));
				}
				fire = true;
				break;
			default:
				break;
			}
		default:
			break;
		}
	}
}

void Input::ActionUpdate(Load &load, bool &run)
{
	// EXPLOSION
	if (load.eTimer > 0)
	{
		load.eTimer -= 1.0f;
	}
	else if (load.eTimer <= 0)
	{
		load.explosionPos.x = 10.0f;
	}
	
	// ENEMY BULLET
	load.enemyBulletPos.y -= 0.05f;

	if (load.bulletControl > 0)
	{
		load.bulletControl -= 1.0f;
	}

	for (int i = 0; i < 36; i++)
	{
		if (load.enemyPos[i].x >= (load.playerX - 0.2f) && load.enemyPos[i].x <= (load.playerX + 0.2f) && load.bulletControl == 0 && load.enemyAlive[i] == true)
		{
			load.enemyBulletPos.x = load.enemyPos[i].x + (0.1055f / 2.0f);
			load.enemyBulletPos.y = load.enemyPos[i].y;
			load.bulletControl = 75.0f;
		}
	}
	
	// CHECK ENEMIES FOR HITTING EDGES
	if (load.enemyInit == true)
	{
		for (int i = 0; i < 36; i++)
		{
			if (load.enemyPos[i].y <= 0.7f && load.enemyAlive[i] == true)
			{
				run = false;
			}
			else if (load.enemyPos[i].x < 0.14f && load.enemyAlive[i] == true)
			{
				enemyRight = true;
				enemyLeft = false;
				for (int z = 0; z < 36; z++)
				{
					load.enemyPos[z].y -= enemyDrop;
				}
			}
			else if (load.enemyPos[i].x > 3.73f && load.enemyAlive[i] == true)
			{
				enemyLeft = true;
				enemyRight = false;
				for (int z = 0; z < 36; z++)
				{
					load.enemyPos[z].y -= enemyDrop;
				}
			}
		}

		// MOVE ENEMIES, SPIN OFF AND ENEMY BULLET COLLISIONS
		for (int i = 0; i < 36; i++)
		{
			bulletTop = { load.bulletX, load.bulletY };

			if (((load.bulletX < load.enemyPos[i].x + 0.2f) && (load.bulletX >= load.enemyPos[i].x)) && ((load.bulletY < load.enemyPos[i].y + 0.13f) && (load.bulletY > load.enemyPos[i].y)) && load.enemyAlive[i] == true)
			{
				load.enemyAlive[i] = false;
				allDead--;
				load.scoreNum += 5;
				printf("Score: %d \n", load.scoreNum);
				if (allDead % 6 == 0)
				{
					enemySpeed = enemySpeed * 1.8f;
				}
				fire = false;
				load.eTimer = load.eHide;
				load.explosionPos = load.enemyPos[i];
			}
			if (enemyRight == true && load.enemyAlive[i] == true)
			{
				load.enemyPos[i].x += enemySpeed;
			}
			else if (enemyLeft == true && load.enemyAlive[i] == true)
			{
				load.enemyPos[i].x -= enemySpeed;
			}
			if (load.enemyAlive[i] == false)
			{
				load.enemyPos[i].y += 0.01f;
			}

			load.angle += 0.1f;

			if (load.angle > 359.9f)
			{
				load.angle = 0.0f;
			}
		}

		// BULLET BARRIER COLLISION
		for (int i = 0; i < 3; i++)
		{
			if (load.health[i] <= 0)
			{
				load.shield[i].y = 10.0f;
			}
			if (((load.bulletX < load.shield[i].x + 0.5f) && (load.bulletX >= load.shield[i].x)) && ((load.bulletY < load.shield[i].y + 0.195f) && (load.bulletY > load.shield[i].y)))
			{
				load.health[i]--;
				fire = false;
			}
			if (((load.enemyBulletPos.x < load.shield[i].x + 0.5f) && (load.enemyBulletPos.x >= load.shield[i].x)) && ((load.enemyBulletPos.y < load.shield[i].y + 0.195f) && (load.enemyBulletPos.y > load.shield[i].y)))
			{
				load.health[i]--;
				load.enemyBulletPos.x = 10.0f;
			}
			for (int j = 0; j < 36; j++)
			{
				if ((((load.enemyPos[j].x < load.shield[i].x + 0.5f) && (load.enemyPos[j].x < load.shield[i].x)) || ((load.enemyPos[j].x + 0.2 < load.shield[i].x + 0.5f) && (load.enemyPos[j].x + 0.2 < load.shield[i].x))) && ((load.enemyPos[j].y < load.shield[i].y + 0.3)))
				{
					load.health[i] -= 0.2f;
				}
			}
		}

		if (load.enemyBulletPos.x < load.playerX + 0.1f && load.enemyBulletPos.x > load.playerX && load.enemyBulletPos.y < 0.58f && load.enemyBulletPos.y > 0.45f)
		{
			load.livesNum--;
			load.enemyBulletPos.x = 10.0f;
			load.eTimer = load.eHide;
			load.explosionPos = { load.playerX , 0.45};
		}
	}

	// ENDS GAME IF ENEMIES DEAD
	if (allDead == 0)
	{
		run = false;
	}
	if (load.livesNum == 0)
	{
		run = false;
	}

	// PLAYER BARRIER COLLISIONS
	if (left == true)
	{
		if (load.playerX <= 0.14f)
		{
			left = false;
		}
		else
		{
			load.playerX -= 0.0175f;
		}
	}

	if (right == true)
	{
		if (load.playerX >= 3.73f)
		{
			right = false;
		}
		else
		{
			load.playerX += 0.0175f;
		}
	}

	// SHOOTING
	if (fire == false)
	{
		load.bulletX = load.playerX + (0.1055f / 2.0f);
		load.bulletY = 0.45f;
	}
	if (fire == true)
	{
		load.bulletX = shotX;
		if (load.bulletY < 4.1f)
		{
			load.bulletY += 0.1f;
		}
		else
		{
			fire = false;
		}
	}

	SDL_itoa(load.scoreNum, sNum, 10);
	for (int i = 7; i < 10; i++)
	{
		load.score[i] = sNum[i - 7];
	}
	load.Text(load.score, load.tScore, load.scoreS);

	SDL_itoa(load.livesNum, lNum, 10);
	for (int i = 7; i < 10; i++)
	{
		load.lives[i] = lNum[i - 7];
	}
	load.Text(load.lives, load.tLives, load.livesS);
}