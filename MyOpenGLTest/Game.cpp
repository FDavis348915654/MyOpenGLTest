#include "Game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
// GLFW
#include <GLFW/glfw3.h>
#include <cmath>

SpriteRenderer  *Renderer;
GameObject *Player;

Game::Game() {

}

Game::Game(GLuint width, GLuint height) : State(GAME_ACTIVE), Keys(), KeysProcessed(), Width(width), Height(height) {

}

Game::~Game() {
	delete Renderer;
}

void Game::Init() {
	// 加载着色器
	ResourceManager::LoadShader("../res/Shaders/lesson_26_sprite.vs", "../res/Shaders/lesson_26_sprite.fs", nullptr, "sprite");
	// 配置着色器, 投影矩阵
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
		static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// 设置专用于渲染的控制
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	// 加载纹理
	ResourceManager::LoadTexture("../res/Texture/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("../res/Texture/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("../res/Texture/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("../res/Texture/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("../res/Texture/paddle.png", true, "paddle");
	// 加载关卡
	GameLevel one; one.Load("../res/levels/one.lvl", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("../res/levels/two.lvl", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("../res/levels/three.lvl", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("../res/levels/four.lvl", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 1;

	glm::vec2 playerPos = glm::vec2(
		this->Width / 2 - PLAYER_SIZE.x / 2,
		this->Height - PLAYER_SIZE.y
	);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
}

void Game::Update(float dt) {

}

void Game::ProcessInput(float dt) {
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// 移动挡板
		if (this->Keys[GLFW_KEY_A])
		{
			Player->Position.x = std::max(Player->Position.x - velocity, 0.0f);
		}
		if (this->Keys[GLFW_KEY_D])
		{
			Player->Position.x = std::min(Player->Position.x + velocity, this->Width - Player->Size.x);
		}
	}
}

void Game::Render() {
	//Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	if (this->State == GAME_ACTIVE)
	{
		// 绘制背景
		Renderer->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
		);
		// 绘制关卡
		this->Levels[this->Level].Draw(*Renderer);
		Player->Draw(*Renderer);
	}
}
