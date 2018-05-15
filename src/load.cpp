#include "load.h"

Load::Load()
{
	enemyInit = false;

	for (int i = 0; i < 36; i++)
	{
		enemyAlive[i] = true;
	}

	Initialise();
	SDL_GetDesktopDisplayMode(0, &screenSize);
	aspectRatio = 4 / 3;
	winW = screenSize.w / 2;
	winH = screenSize.h / 2;
	CreateWindow();
	CreateOpenGL();
	InitGL();
}

void Load::Initialise()
{
	// SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n", SDL_GetError());
	}
	SDL_Log("SDL initialised OK!\n");

	// SDL_Image
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_image init error: %s\n", IMG_GetError());
	}
	SDL_Log("SDL_image initialised OK!\n");

	// SDL_ttf
	if (TTF_Init() == -1)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_ttf init error: %s\n", SDL_GetError());
	}
	SDL_Log("SDL_ttf initialised OK!\n");

	// SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_mixer init error: %s\n   PERHAPS NO HEADPHONES/SPEAKERS CONNECTED\n", Mix_GetError());
	}
	SDL_Log("SDL_mixer initialised OK!\n");
}

void Load::CreateWindow()
{
	win = SDL_CreateWindow("Daniel Downes | Graphics CGP2012M | 14520077", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winW, winH, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (win == nullptr)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow init error: %s\n", SDL_GetError());
	}
}

void Load::CreateOpenGL()
{
	// Set OpenGL context parameters
	int major = 4, minor = 3;
	SDL_Log("Asking for OpenGL %d.%d context\n", major, minor);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_CORE); //use core profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); //ask for forward compatible
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //default, probably

	glcontext = SDL_GL_CreateContext(win); // OpenGL Context Creation
	if (glcontext == NULL)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_GL_CreateContext init error: %s\n", SDL_GetError());
	}
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
	SDL_Log("Got an OpenGL %d.%d context\n", major, minor);

	GLenum rev; // initialise GLEW - sets up the OpenGL function pointers for the version of OpenGL we're using
	glewExperimental = GL_TRUE; //https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
	rev = glewInit();
	if (GLEW_OK != rev)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "glewInit error: %s\n", glewGetErrorString(rev));
	}
	SDL_Log("glew initialised OK!\n");

	glViewport(0, 0, winW, winH);
}

void Load::InitGL()
{
	//SPECIFIC VALUES
	GLfloat Vertices[] = {
		// Positions          // Texture Coords
		0.0f,	0.2f,	0.0f,		0.0f, 1.0f,    // Top Left 
		0.13f,	0.2f,	0.0f,		1.0f, 1.0f,   // Top Right
		0.13f,	0.0f,	0.0f,		1.0f, 0.0f,   // Bottom Right
		0.0f,	0.0f,	0.0f,		0.0f, 0.0f   // Bottom Left
	};

	GLuint Indices[] = {
		0, 1, 2,  // First Triangle
		0, 2, 3,  // Second Triangle
	};

	// LOAD TEXTURE TO SURFACE
	SDL_Surface* enemy = IMG_Load("..\\assets\\enemy.png");
	if (enemy == NULL)
	{
		std::cout << "Image load failed...." << std::endl;
	}

	SDL_Surface* enemyBullet = IMG_Load("..\\assets\\enemyBullet.png");
	if (enemy == NULL)
	{
		std::cout << "Image load failed...." << std::endl;
	}

	SDL_Surface* player = IMG_Load("..\\assets\\player.png");
	if (enemy == NULL)
	{
		std::cout << "Image load failed...." << std::endl;
	}

	SDL_Surface* playerBullet = IMG_Load("..\\assets\\playerBullet.png");
	if (enemy == NULL)
	{
		std::cout << "Image load failed...." << std::endl;
	}

	SDL_Surface* background = IMG_Load("..\\assets\\background.png");
	if (enemy == NULL)
	{
		std::cout << "Image load failed...." << std::endl;
	}

	SDL_Surface* frame = IMG_Load("..\\assets\\frame.png");
	if (enemy == NULL)
	{
		std::cout << "Image load failed...." << std::endl;
	}

	SDL_Surface* shieldFull = IMG_Load("..\\assets\\shieldfull.png");
	if (enemy == NULL)
	{
		std::cout << "Image load failed...." << std::endl;
	}

	SDL_Surface* shieldMid = IMG_Load("..\\assets\\shieldmid.png");
	if (enemy == NULL)
	{
		std::cout << "Image load failed...." << std::endl;
	}

	SDL_Surface* shieldLast = IMG_Load("..\\assets\\shieldlast.png");
	if (enemy == NULL)
	{
		std::cout << "Image load failed...." << std::endl;
	}

	SDL_Surface* explosion = IMG_Load("..\\assets\\explode.png");
	if (enemy == NULL)
	{
		std::cout << "Image load failed...." << std::endl;
	}

	const GLchar* vertexSource[] =
	{
		"#version 440 core\n"
		"layout(location = 0) in vec3 position;\n"
		"layout (location = 1) in vec2 texCoord;\n"
		"out vec2 TexCoord;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{ gl_Position = projection * view * model * vec4(position.x, position.y, position.z, 1.0);\n"
		"TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);}"
	};

	const GLchar* fragmentSource[] =
	{
		"#version 440 core\n"
		"in vec2 TexCoord;\n"
		"out vec4 color;\n"
		"uniform sampler2D ourTexture;\n"
		"void main()\n"
		"{ color = texture(ourTexture, TexCoord);}"
	};

	// BUFFERS
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, vertexSource, NULL);
	glCompileShader(vertex);

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, fragmentSource, NULL);
	glCompileShader(fragment);

	shaderProgram = glCreateProgram();

	// DEBUGGING
	GLchar infoLog[512];
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
	}
	else
	{
		std::cout << "Vertex shader compiled OK" << std::endl;
		glAttachShader(shaderProgram, vertex);
	}
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
	{
		std::cout << "Fragment shader compiled OK" << std::endl;
		glAttachShader(shaderProgram, fragment);
	}

	// LINK AND CLEANUP
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);



	// ENEMY TEXTURE PARAMETERS
	glGenTextures(1, &tEnemy);
	glBindTexture(GL_TEXTURE_2D, tEnemy);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, enemy->w, enemy->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, enemy->pixels);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(enemy);

	// BACKGROUND
	glGenTextures(1, &tBackground);
	glBindTexture(GL_TEXTURE_2D, tBackground);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, background->w, background->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, background->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(background);

	// FRAME
	glGenTextures(1, &tFrame);
	glBindTexture(GL_TEXTURE_2D, tFrame);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame->w, frame->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(frame);

	// PLAYER TEXTURE PARAMATERS
	glGenTextures(1, &tPlayer);
	glBindTexture(GL_TEXTURE_2D, tPlayer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, player->w, player->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, player->pixels);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(player);

	// PLAYER BULLET TEXTURE PARAMETERS
	glGenTextures(1, &tPlayerBullet);
	glBindTexture(GL_TEXTURE_2D, tPlayerBullet);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, playerBullet->w, playerBullet->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, playerBullet->pixels);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(playerBullet);

	// ENEMY BULLET
	glGenTextures(1, &tEnemyBullet);
	glBindTexture(GL_TEXTURE_2D, tEnemyBullet);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, enemyBullet->w, enemyBullet->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, enemyBullet->pixels);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(enemyBullet);

	// SHIELD FULL
	glGenTextures(1, &tShieldFull);
	glBindTexture(GL_TEXTURE_2D, tShieldFull);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, shieldFull->w, shieldFull->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, shieldFull->pixels);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(shieldFull);

	// SHIELD MID
	glGenTextures(1, &tShieldMid);
	glBindTexture(GL_TEXTURE_2D, tShieldMid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, shieldMid->w, shieldMid->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, shieldMid->pixels);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(shieldMid);

	// SHIELD LAST
	glGenTextures(1, &tShieldLast);
	glBindTexture(GL_TEXTURE_2D, tShieldLast);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, shieldLast->w, shieldLast->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, shieldLast->pixels);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(shieldLast);

	// EXPLOSION
	glGenTextures(1, &tExplosion);
	glBindTexture(GL_TEXTURE_2D, tExplosion);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, explosion->w, explosion->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, explosion->pixels);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(explosion);

	//Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	std::cout << glGetError() << std::endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//orthographic projection
	//left, right, bottom, top, near clip plane, far clip plane
	projection = glm::ortho(0.0f, 4.0f, 0.0f, 3.0f, -1.0f, 100.0f);
	//projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenSize.w / (GLfloat)screenSize.h, 0.1f, 100.0f);
	projectionLocation = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

void Load::Render()
{
	// Clear the back buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//// OBJECT ONE
	//model = glm::mat4();
	//model = glm::translate(model, glm::vec3(3.87f, 2.8f, -10.0f));
	//GLint modelLocation = glGetUniformLocation(shaderProgram, "model");
	//GLint viewLocation = glGetUniformLocation(shaderProgram, "view");
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	//glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glUseProgram(shaderProgram);
	viewLocation = glGetUniformLocation(shaderProgram, "view");

	// BACKGROUND
	glBindTexture(GL_TEXTURE_2D, tBackground);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
	model = glm::scale(model, glm::vec3(35.0f, 15.0f, 0.0f));
	modelLocation = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	// PLAYER BULLET
	glBindTexture(GL_TEXTURE_2D, tPlayerBullet);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(bulletX, bulletY, -10.0f));
	model = glm::scale(model, glm::vec3(0.18f, 0.8f, 0.0f));
	modelLocation = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);
	
	// ENEMY BULLET
	glBindTexture(GL_TEXTURE_2D, tEnemyBullet);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(enemyBulletPos.x, enemyBulletPos.y, -10.0f));
	model = glm::scale(model, glm::vec3(0.18f, 0.8f, 0.0f));
	modelLocation = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);
	
	// BARRIERS
	for (int i = 0; i < 3; i++)
	{
		if (health[i] >= 5.0f && health[i] != 0)
		{
			glBindTexture(GL_TEXTURE_2D, tShieldFull);

			model = glm::mat4();
			model = glm::translate(model, glm::vec3(shield[i].x, shield[i].y, -10.0f));
			model = glm::scale(model, glm::vec3(4.0f, 1.5f, 0.0f));
			modelLocation = glGetUniformLocation(shaderProgram, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);
		}
		else if ((health[i] > 2.1f && health[i] < 5.0f) && health[i] != 0)
		{
			glBindTexture(GL_TEXTURE_2D, tShieldMid);

			model = glm::mat4();
			model = glm::translate(model, glm::vec3(shield[i].x, shield[i].y, -10.0f));
			model = glm::scale(model, glm::vec3(4.0f, 1.5f, 0.0f));
			modelLocation = glGetUniformLocation(shaderProgram, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);
		}
		else if (health[i] < 2.1f && health[i] != 0)
		{
			glBindTexture(GL_TEXTURE_2D, tShieldLast);

			model = glm::mat4();
			model = glm::translate(model, glm::vec3(shield[i].x, shield[i].y, -10.0f));
			model = glm::scale(model, glm::vec3(4.0f, 1.5f, 0.0f));
			modelLocation = glGetUniformLocation(shaderProgram, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);
		}
	}

	// ENEMIES		
	glBindTexture(GL_TEXTURE_2D, tEnemy);
	if (enemyInit == false)
	{
		for (float y = 1.9f; y < 2.7f; y += 0.25f)
		{
			for (float x = 0.15f; x < 2.35f; x += 0.25f)
			{
				enemyPos[counter] = { x, y };
				model = glm::mat4();
				model = glm::translate(model, glm::vec3(enemyPos[counter], -10.0f));
				modelLocation = glGetUniformLocation(shaderProgram, "model");
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
				glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);
				counter++;
			}
		}
		enemyInit = true;
	}
	else
	{
		for (counter = 0; counter < 36; counter++)
		{
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(enemyPos[counter], -10.0f));
			if (enemyAlive[counter] == false)
			{
				model = glm::rotate(model, glm::radians(angle), glm::vec3(50.0f, 50.0f, 0.0f));
			}
			modelLocation = glGetUniformLocation(shaderProgram, "model");
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);
		}
	}

	//// WORKING BLOCK
	//for (float y = 1.8f; y < 2.6f; y += 0.25f)
	//{
	//	for (float x = 0.1f; x < 2.3f; x += 0.25f)
	//	{
	//		model = glm::mat4();
	//		model = glm::translate(model, glm::vec3(x, y, -10.0f));
	//		modelLocation = glGetUniformLocation(shaderProgram, "model");
	//		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	//		glBindVertexArray(VAO);
	//		glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);
	//	}
	//}

	//	PLAYER
	glBindTexture(GL_TEXTURE_2D, tPlayer);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(playerX, 0.45f, -10.0f));
	modelLocation = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	// EXPLOSION
	glBindTexture(GL_TEXTURE_2D, tExplosion);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(explosionPos.x, explosionPos.y, -10.0f));
	modelLocation = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	// FRAME
	glBindTexture(GL_TEXTURE_2D, tFrame);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-0.1f, -0.1f, -10.0f));
	model = glm::scale(model, glm::vec3(32.3f, 15.7f, 0.0f));
	modelLocation = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	// SCORE
	glBindTexture(GL_TEXTURE_2D, tScore);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.12f, 0.03f, -10.0f));
	model = glm::scale(model, glm::vec3(5.0f, 2.0f, 0.0f));
	modelLocation = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	// LIVES
	glBindTexture(GL_TEXTURE_2D, tLives);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(3.25f, 0.03f, -10.0f));
	model = glm::scale(model, glm::vec3(5.0f, 2.0f, 0.0f));
	modelLocation = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 1000, GL_UNSIGNED_INT, 0);

	//set up the rotation transformation matrix
	//_transRotate = glm::rotate(_transRotate, glm::radians(1.0f), glm::vec3(5.0f, 0.0f, 0.0f));
	//GLint transLocation = glGetUniformLocation(shaderProgram, "projection");
	//glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(_transTranslate*_transRotate*_transScale));

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glBindVertexArray(0);
	SDL_GL_SwapWindow(win);
}

void Load::Text(char* mess, GLuint &tex, SDL_Surface* surf)
{
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	TTF_Font *font = TTF_OpenFont("..\\assets\\KenVector Future.ttf", 100);
	surf = TTF_RenderText_Blended(font, mess, textColor);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(surf);	
	TTF_CloseFont(font);
}

Load::~Load()
{
	SDL_Log("Finished. Cleaning up and closing down\n");

	//Mix_FreeChunk(scratch);
	//Mix_FreeMusic(music);
	//Mix_CloseAudio();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	SDL_GL_DeleteContext(glcontext);
	SDL_Quit();
}