#include "Game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
// GLFW
#include <GLFW/glfw3.h>
#include <cmath>

SpriteRenderer  *Renderer;
GameObject *Player;
BallObject *Ball;
ParticleGenerator *Particles;
PostProcessor *Effects;
GLfloat ShakeTime = 0.0f;

GLboolean CheckCollision(GameObject &one, GameObject &two);
Collision CheckCollision(BallObject &one, GameObject &two); // AABB - Circle collision
Direction VectorDirection(glm::vec2 target);

Game::Game() {

}

Game::Game(GLuint width, GLuint height) : State(GAME_ACTIVE), Keys(), KeysProcessed(), Width(width), Height(height) {

}

Game::~Game() {
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
}

void Game::Init() {
	// ������ɫ��
	ResourceManager::LoadShader("../res/Shaders/lesson_26_sprite.vs", "../res/Shaders/lesson_26_sprite.fs", nullptr, "sprite");
	ResourceManager::LoadShader("../res/Shaders/lesson_26_particle.vs", "../res/Shaders/lesson_26_particle.fs", nullptr, "particle");
	ResourceManager::LoadShader("../res/Shaders/lesson_26_post_processing.vs", "../res/Shaders/lesson_26_post_processing.fs", nullptr, "postprocessing");
	// ������ɫ��, ͶӰ����
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
		static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
	// ����ר������Ⱦ�Ŀ���
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	// ��������
	ResourceManager::LoadTexture("../res/Texture/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("../res/Texture/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("../res/Texture/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("../res/Texture/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("../res/Texture/paddle.png", true, "paddle");
	ResourceManager::LoadTexture("../res/Texture/particle.png", GL_TRUE, "particle");
	// ���عؿ�
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
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
		ResourceManager::GetTexture("face"));
	// ��������������
	Particles = new ParticleGenerator(
		ResourceManager::GetShader("particle"),
		ResourceManager::GetTexture("particle"),
		500
	);
	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
}

void Game::Update(float dt) {
	// ���¶���
	Ball->Move(dt, this->Width);
	// �����ײ
	DoCollisions();

	if (Ball->Position.y >= this->Height) // ���Ƿ�Ӵ��ײ��߽磿
	{
		this->ResetLevel();
		this->ResetPlayer();
	}

	// Update particles
	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));

	// Reduce shake time
	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
			Effects->Shake = GL_FALSE;
	}
}

void Game::ProcessInput(float dt) {
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// �ƶ�����
		if (this->Keys[GLFW_KEY_A])
		{
			float dx = std::max(Player->Position.x - velocity, 0.0f) - Player->Position.x;
			Player->Position.x += dx;
			if (Ball->Stuck)
				Ball->Position.x += dx;
		}
		if (this->Keys[GLFW_KEY_D])
		{
			float dx = std::min(Player->Position.x + velocity, this->Width - Player->Size.x) - Player->Position.x;
			Player->Position.x += dx;
			if (Ball->Stuck)
				Ball->Position.x += dx;
		}
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->Stuck = false;
	}
}

void Game::Render() {
	//Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	if (this->State == GAME_ACTIVE)
	{
		Effects->BeginRender();
		// ���Ʊ���
		Renderer->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
		);
		// ���ƹؿ�
		this->Levels[this->Level].Draw(*Renderer);
		Player->Draw(*Renderer);
		Particles->Draw();
		Ball->Draw(*Renderer);
		Effects->EndRender();
		Effects->Render(glfwGetTime());
	}
}

void Game::DoCollisions() {
	for (GameObject &box : this->Levels[this->Level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);
			if (std::get<0>(collision)) // ���collision �� true
			{
				// ���ש�鲻��ʵ�ľ�����ש��
				if (!box.IsSolid)
					box.Destroyed = GL_TRUE;
				else {
					// if block is solid, enable shake effect
					ShakeTime = 0.05f;
					Effects->Shake = GL_TRUE;
				}
				// ��ײ����
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (dir == Direction::LEFT || dir == Direction::RIGHT) // ˮƽ������ײ
				{
					Ball->Velocity.x = -Ball->Velocity.x; // ��תˮƽ�ٶ�
					// �ض�λ
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
					if (dir == Direction::LEFT)
						Ball->Position.x += penetration; // ��������
					else
						Ball->Position.x -= penetration; // ��������
				}
				else // ��ֱ������ײ
				{
					Ball->Velocity.y = -Ball->Velocity.y; // ��ת��ֱ�ٶ�
					// �ض�λ
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
					if (dir == Direction::UP)
						Ball->Position.y -= penetration; // ��������
					else
						Ball->Position.y += penetration; // ��������
				}
			}

			Collision result = CheckCollision(*Ball, *Player);
			if (!Ball->Stuck && std::get<0>(result))
			{
				// ��������˵�����ĸ�λ�ã������������ĸ�λ�����ı��ٶ�
				GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
				GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
				GLfloat percentage = distance / (Player->Size.x / 2);
				// ���ݽ���ƶ�
				GLfloat strength = 2.0f;
				glm::vec2 oldVelocity = Ball->Velocity;
				Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
				//Ball->Velocity.y = -Ball->Velocity.y;
				Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
				Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
			}
		}
	}
}

// AABB ��ײ
GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
	// x�᷽����ײ��
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// y�᷽����ײ��
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// ֻ��������������ײʱ����ײ
	return collisionX && collisionY;
}

// Բ��ײ���
Collision CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
{
	// ��ȡԲ������ 
	glm::vec2 center(one.Position + one.Radius);
	// ����AABB����Ϣ�����ġ���߳���
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
	glm::vec2 aabb_center(
		two.Position.x + aabb_half_extents.x,
		two.Position.y + aabb_half_extents.y
	);
	// ��ȡ�������ĵĲ�ʸ��
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// AABB_center����clamped�����͵õ�����ײ���Ͼ���Բ����ĵ�closest
	glm::vec2 closest = aabb_center + clamped;
	// ���Բ��center�������closest��ʸ�����ж��Ƿ� length <= radius
	difference = closest - center;
	if (glm::length(difference) <= one.Radius)
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	else
		return std::make_tuple(GL_FALSE, Direction::UP, glm::vec2(0, 0));
}

Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),  // ��
		glm::vec2(1.0f, 0.0f),  // ��
		glm::vec2(0.0f, -1.0f), // ��
		glm::vec2(-1.0f, 0.0f)  // ��
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

void Game::ResetLevel()
{
	if (this->Level == 0)this->Levels[0].Load("levels/one.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 1)
		this->Levels[1].Load("levels/two.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 2)
		this->Levels[2].Load("levels/three.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 3)
		this->Levels[3].Load("levels/four.lvl", this->Width, this->Height * 0.5f);
}

void Game::ResetPlayer()
{
	// Reset player/ball stats
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}
