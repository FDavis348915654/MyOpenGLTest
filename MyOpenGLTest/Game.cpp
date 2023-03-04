#include "Game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
// GLFW
#include <GLFW/glfw3.h>
#include <cmath>
#include <irrklang/irrKlang.h>
using namespace irrklang;

bool TestLevel = true;
SpriteRenderer  *Renderer;
GameObject *Player;
BallObject *Ball;
ParticleGenerator *Particles;
PostProcessor *Effects;
GLfloat ShakeTime = 0.0f;
ISoundEngine *SoundEngine = NULL;
TextRenderer *Text;

GLboolean CheckCollision(GameObject &one, GameObject &two);
Collision CheckCollision(BallObject &one, GameObject &two); // AABB - Circle collision
Direction VectorDirection(glm::vec2 target);
void ActivatePowerUp(PowerUp &powerUp);
GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);

Game::Game(GLuint width, GLuint height) : State(GAME_MENU), Keys(), KeysProcessed(), Width(width), Height(height) {

}

Game::~Game() {
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
	delete Text;
	if (SoundEngine) {
		SoundEngine->drop();
		SoundEngine = NULL;
	}
}

void Game::Init() {
	SoundEngine = createIrrKlangDevice();
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
	ResourceManager::LoadTexture("../res/Texture/powerup_speed.png", true, "powerup_speed");
	ResourceManager::LoadTexture("../res/Texture/powerup_sticky.png", true, "powerup_sticky");
	ResourceManager::LoadTexture("../res/Texture/powerup_increase.png", true, "powerup_increase");
	ResourceManager::LoadTexture("../res/Texture/powerup_confuse.png", true, "powerup_confuse");
	ResourceManager::LoadTexture("../res/Texture/powerup_chaos.png", true, "powerup_chaos");
	ResourceManager::LoadTexture("../res/Texture/powerup_passthrough.png", true, "powerup_passthrough");
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
	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("../res/fonts/OCRAEXT.TTF", 24);

	// ���ű�������
	SoundEngine->play2D("../res/audio/breakout.mp3", GL_TRUE);
}

void Game::Update(float dt) {
	// ���¶���
	Ball->Move(dt, this->Width);
	// �����ײ
	DoCollisions();

	if (Ball->Position.y >= this->Height && !TestLevel) // ���Ƿ�Ӵ��ײ��߽磿
	{
		--this->Lives;
		// ����Ƿ���ʧȥ��������ֵ? : ��Ϸ����
		if (this->Lives == 0)
		{
			this->ResetLevel();
			this->State = GAME_MENU;
		}
		this->ResetPlayer();
	}
	// ����ʱ�߽練��
	if (TestLevel && Ball->Position.y + Ball->Radius * 2 > this->Height) {
		Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
	}

	// Update particles
	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
	// update PowerUps
	this->UpdatePowerUps(dt);

	// Reduce shake time
	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
			Effects->Shake = GL_FALSE;
	}

	if (this->State == GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
	{
		this->ResetLevel();
		this->ResetPlayer();
		Effects->Chaos = GL_TRUE;
		this->State = GAME_WIN;
	}
}

void Game::ProcessInput(float dt) {
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// �ƶ�����
		if (this->Keys[GLFW_KEY_A] || this->Keys[GLFW_KEY_LEFT])
		{
			float dx = std::max(Player->Position.x - velocity, 0.0f) - Player->Position.x;
			Player->Position.x += dx;
			if (Ball->Stuck)
				Ball->Position.x += dx;
		}
		if (this->Keys[GLFW_KEY_D] || this->Keys[GLFW_KEY_RIGHT])
		{
			float dx = std::min(Player->Position.x + velocity, this->Width - Player->Size.x) - Player->Position.x;
			Player->Position.x += dx;
			if (Ball->Stuck)
				Ball->Position.x += dx;
		}
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->Stuck = false;
	}

	if (this->State == GAME_MENU)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
		}
		if ((this->Keys[GLFW_KEY_W] || this->Keys[GLFW_KEY_UP]) && !(this->KeysProcessed[GLFW_KEY_W] && this->KeysProcessed[GLFW_KEY_UP]))
		{
			this->Level = (this->Level + 1) % 4;
			this->KeysProcessed[GLFW_KEY_W] = GL_TRUE;
			this->KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
		}
		if ((this->Keys[GLFW_KEY_S] || this->Keys[GLFW_KEY_DOWN]) && !(this->KeysProcessed[GLFW_KEY_S] && this->KeysProcessed[GLFW_KEY_DOWN]))
		{
			if (this->Level > 0)
				--this->Level;
			else
				this->Level = 3;
			this->KeysProcessed[GLFW_KEY_S] = GL_TRUE;
			this->KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
		}
	}

	if (this->State == GAME_WIN)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			Effects->Chaos = GL_FALSE;
			this->State = GAME_MENU;
		}
	}
}

void Game::Render() {
	//Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	if (this->State == GAME_ACTIVE || this->State == GAME_MENU)
	{
		// ����: �������������ǰ��
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
		for (PowerUp &powerUp : this->PowerUps) {
			if (!powerUp.Destroyed) {
				powerUp.Draw(*Renderer);
			}
		}

		// ����: ��غ������������
		Effects->EndRender();
		Effects->Render(glfwGetTime());

		std::stringstream ss; ss << this->Lives;
		Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
	}

	if (this->State == GAME_WIN)
	{
		Text->RenderText(
			"You WON!!!", 320.0, Height / 2 - 20.0, 1.0, glm::vec3(0.0, 1.0, 0.0)
		);
		Text->RenderText(
			"Press ENTER to retry or ESC to quit", 130.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0)
		);
	}

	if (this->State == GAME_MENU)
	{
		Text->RenderText("Press ENTER to start", 250.0f, Height / 2, 1.0f);
		Text->RenderText("Press W or S to select level", 245.0f, Height / 2 + 20.0f, 0.75f);
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
				if (!box.IsSolid) {
					box.Destroyed = GL_TRUE;
					this->SpawnPowerUps(box);
					SoundEngine->play2D("../res/audio/bleep.mp3", GL_FALSE);
				}
				else {
					// if block is solid, enable shake effect
					ShakeTime = 0.05f;
					Effects->Shake = GL_TRUE;
					SoundEngine->play2D("../res/audio/solid.wav", GL_FALSE);
				}
				// ��ײ����
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (!(Ball->PassThrough && !box.IsSolid)) {
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
			}

			Collision result = CheckCollision(*Ball, *Player);
			if (!Ball->Stuck && std::get<0>(result))
			{
				// ��������˵�����ĸ�λ�ã������������ĸ�λ�����ı��ٶ�
				GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
				GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
				GLfloat percentage = distance / (Player->Size.x / 2);
				// ���ݽ���ƶ�
				GLfloat strength = 3.0f;
				glm::vec2 oldVelocity = Ball->Velocity;
				Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
				//Ball->Velocity.y = -Ball->Velocity.y;
				Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
				Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
				Ball->Stuck = Ball->Sticky;

				SoundEngine->play2D("../res/audio/bleep.wav", GL_FALSE);
			}
		}
	}

	for (PowerUp &powerUp : this->PowerUps)
	{
		if (!powerUp.Destroyed)
		{
			if (powerUp.Position.y >= this->Height)
				powerUp.Destroyed = GL_TRUE;
			if (CheckCollision(*Player, powerUp))
			{   // �����뵲��Ӵ�����������
				ActivatePowerUp(powerUp);
				powerUp.Destroyed = GL_TRUE;
				powerUp.Activated = GL_TRUE;
				SoundEngine->play2D("../res/audio/powerup.wav", GL_FALSE);
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

	this->Lives = 3;
}

void Game::ResetPlayer()
{
	// Reset player/ball stats
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}

GLboolean ShouldSpawn(GLuint chance)
{
	GLuint random = rand() % chance;
	return random == 0;
}

void Game::SpawnPowerUps(GameObject &block)
{
	if (ShouldSpawn(5)) // 1/75�ļ���
		this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed")));
	if (ShouldSpawn(25))
		this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")));
	if (ShouldSpawn(15))
		this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough")));
	if (ShouldSpawn(30)) // if (ShouldSpawn(75))
		this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase")));
	if (ShouldSpawn(75)) // �������
		this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")));
	if (ShouldSpawn(65)) // �������
		this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos")));
}

void ActivatePowerUp(PowerUp &powerUp)
{
	// ���ݵ������ͷ�������
	if (powerUp.Type == "speed")
	{
		Ball->Velocity *= 1.2;
	}
	else if (powerUp.Type == "sticky")
	{
		Ball->Sticky = GL_TRUE;
		Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.Type == "pass-through")
	{
		Ball->PassThrough = GL_TRUE;
		Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.Type == "pad-size-increase")
	{
		Player->Size.x += 50;
	}
	else if (powerUp.Type == "confuse")
	{
		if (!Effects->Chaos)
			Effects->Confuse = GL_TRUE; // ֻ��chaosδ����ʱ��Ч��chaosͬ��
	}
	else if (powerUp.Type == "chaos")
	{
		if (!Effects->Confuse)
			Effects->Chaos = GL_TRUE;
	}
}

void Game::UpdatePowerUps(GLfloat dt)
{
	for (PowerUp &powerUp : this->PowerUps)
	{
		powerUp.Position += powerUp.Velocity * dt;
		if (powerUp.Activated)
		{
			powerUp.Duration -= dt;

			if (powerUp.Duration <= 0.0f)
			{
				// ֮��Ὣ��������Ƴ�
				powerUp.Activated = GL_FALSE;
				// ͣ��Ч��
				if (powerUp.Type == "sticky")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
					{   // ����û������stickyЧ�����ڼ���״̬ʱ���ã�����ͬ��
						Ball->Sticky = GL_FALSE;
						Player->Color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.Type == "pass-through")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
					{
						Ball->PassThrough = GL_FALSE;
						Ball->Color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.Type == "confuse")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
					{
						Effects->Confuse = GL_FALSE;
					}
				}
				else if (powerUp.Type == "chaos")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
					{
						Effects->Chaos = GL_FALSE;
					}
				}
			}
		}
	}
	this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
		[](const PowerUp &powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
	), this->PowerUps.end());
}

GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
	for (const PowerUp &powerUp : powerUps)
	{
		if (powerUp.Activated)
			if (powerUp.Type == type)
				return GL_TRUE;
	}
	return GL_FALSE;
}
