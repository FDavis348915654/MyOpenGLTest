#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <random>
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

// ��������Ϸ�ĵ�ǰ״̬
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

// ��ʼ������Ĵ�С
const glm::vec2 PLAYER_SIZE(100, 20);
// ��ʼ�����������
const GLfloat PLAYER_VELOCITY(500.0f);

// ��ʼ������ٶ�
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// ��İ뾶
const GLfloat BALL_RADIUS = 12.5f;

class Game
{
public:
	// ��Ϸ״̬
	GameState State;
	GLboolean Keys[1024];
	GLboolean KeysProcessed[1024];
	GLuint Width, Height;

	std::vector<GameLevel> Levels;
	GLuint Level;

	// ���캯��/��������
	Game();
	Game(GLuint width, GLuint height);
	~Game();
	// ��ʼ����Ϸ״̬���������е���ɫ��/����/�ؿ���
	void Init();
	// ��Ϸѭ��
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
};
