#include "Game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
// GLFW
#include <GLFW/glfw3.h>

SpriteRenderer  *Renderer;

Game::Game() {

}

Game::Game(GLuint width, GLuint height) : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {

}

Game::~Game() {
	delete Renderer;
}

void Game::Init() {
	// ������ɫ��
	ResourceManager::LoadShader("../res/Shaders/lesson_26_sprite.vs", "../res/Shaders/lesson_26_sprite.fs", nullptr, "sprite");
	// ������ɫ��, ͶӰ����
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
		static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// ����ר������Ⱦ�Ŀ���
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	// ��������
	ResourceManager::LoadTexture("../res/Texture/awesomeface.png", GL_TRUE, "face");
}

void Game::Update(float dt) {

}

void Game::ProcessInput(float dt) {

}

void Game::Render() {
	Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
