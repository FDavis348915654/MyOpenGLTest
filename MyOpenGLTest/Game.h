#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <random>
#include <tuple>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Model.h"
#include "stb_image.h"
#include "SimpleDrawTestBase.h"
#include "Camera.h"
#include "game_level.h"
#include "ball_object.h"
#include "particle_generator.h"
#include "post_processor.h"
#include "power_up.h"
#include "text_renderer.h"

// 代表了游戏的当前状态
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum class Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

// 初始化挡板的大小
const glm::vec2 PLAYER_SIZE(100, 20);
// 初始化当班的速率
const GLfloat PLAYER_VELOCITY(500.0f);

// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// 球的半径
const GLfloat BALL_RADIUS = 12.5f;

class Game
{
public:
	// 游戏状态
	GameState State;
	GLboolean Keys[1024];
	GLboolean KeysProcessed[1024];
	GLuint Width, Height;

	std::vector<GameLevel> Levels;
	GLuint Level;
	std::vector<PowerUp> PowerUps;
	GLuint Lives;

	// 构造函数/析构函数
	Game(GLuint width, GLuint height);
	~Game();
	// 初始化游戏状态（加载所有的着色器/纹理/关卡）
	void Init();
	// 游戏循环
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
	void DoCollisions();

	// Reset
	void ResetLevel();
	void ResetPlayer();

	// Powerups
	void SpawnPowerUps(GameObject &block);
	void UpdatePowerUps(GLfloat dt);
};
