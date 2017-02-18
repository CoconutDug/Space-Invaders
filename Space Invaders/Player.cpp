#include "Player.h"

Player::Player(DSound& audio, int posX, int posY)
	:
	posX(posX),
	posY(posY),
	key(WHITE),
	aSurface(Surface::FromFile(L"Player\\Player.bmp")),
	bullet(new Bullet(audio, posX + 12, posY)),
	AABB(posY, posY + aSurface.GetHeight(), posX, posX + aSurface.GetWidth()),
	bulletFired(false),
	playerHit(false),
	playerAlive(true)
{
	std::ifstream is("save.txt", std::ios::in | std::ios::binary); //create input file stream
	if (is.is_open()) //if file is open
	{
		ScoreAndLivesFromFile(is);
	}
	else
	{
		score = 0;
		lives = 3;
		ScoreAndLivesToFile();
	}
	is.close();//close file
	explosion = audio.CreateSound("Explosion.wav");
}

Player::~Player()
{
	delete bullet;
}

void Player::Draw(D3DGraphics& gfx)
{
	for (int y = 0; y < (int)aSurface.GetHeight(); y++)
	{
		for (int x = 0; x < (int)aSurface.GetWidth(); x++)
		{
			if (key != aSurface.GetPixel(x, y))
			{
				gfx.PutPixel(x + posX, y + posY, aSurface.GetPixel(x, y));
			}
		}
	}
	//AABB.Draw(gfx);
	if (bulletFired == true)
	{
		bullet->GetDrawable().Rasterize(gfx);
	}
}

void Player::Update(KeyboardClient& kbd)
{
	if (kbd.KeyIsPressed(VK_LEFT))
	{
		MoveLeft();
	}
	else if (kbd.KeyIsPressed(VK_RIGHT))
	{
		MoveRight();
	}
}

void Player::MoveLeft()
{
	int width = aSurface.GetWidth();
	posX -= 2;
	if (posX <= 0)
	{
		posX = 0;
	}
	AABB.left = posX;
	AABB.right = posX + aSurface.GetWidth();
}
void Player::MoveRight()
{
	int width = aSurface.GetWidth();
	posX += 2;
	if (posX + width >= 799)
	{
		posX = 799 - width;
	}
	AABB.left = posX;
	AABB.right = posX + aSurface.GetWidth();
}

void Player::FireBullet()
{
	if (bulletFired == false)
	{
		CreateBullet();
		UpdateBullet();
		bulletFired = true;
	}
}
void Player::CreateBullet()
{
	bullet->SetPosX(posX + 12);
	bullet->SetPosY(posY);
	bullet->BulletIsFired();
}
void Player::UpdateBullet()
{
	if (bulletFired == true)
	{
		if (!bullet->UpdatePlayerBullet(posX + 12, posY))
		{
			bulletFired = false;
		}
	}
}

int& Player::GetScore()
{
	return score;
}

int& Player::GetLives()
{
	return lives;
}

Bullet* Player::GetPlayerBullet()
{
	return bullet;
}

RectI Player::GetAABB()
{
	return AABB;
}

void Player::IncreaseScore()
{
	score++;
}

void Player::PlayerHit()
{
	explosion.Play();
	playerHit = true;
}

void Player::PlayerNotHit()
{
	playerHit = false;
}

void Player::LoseLife()
{
	lives--;
}

bool Player::IsPlayerHit()
{
	return playerHit;
}

void Player::ResetPlayer()
{
	static int count = 300;
	if (count <= 0)
	{
		posX = 386;
		posY = 580;
		AABB.left = posX;
		AABB.right = posX + aSurface.GetWidth();
		AABB.top = posY;
		AABB.bottom = posY + aSurface.GetHeight();
		playerHit = false;
		LoseLife();
		count = 300;
	}
	else
	{
		count--;
	}
}

void Player::PlayLaserSound()
{
	if (bulletFired == false)
	{
		bullet->PlayLaserSound();
	}
}

void Player::ResetScoreAndLives()
{
	score = 0;
	lives = 3;
	ScoreAndLivesToFile();
}

bool Player::IsPlayerDead()
{
	if (lives <= 0)
	{
		return playerAlive;
	}
	else
	{
		return !playerAlive;
	}
}
void Player::ScoreAndLivesToFile()
{
	std::ofstream os("save.txt", std::ios::binary | std::ios::out);
	if (os.is_open()) //If file was opened successfully for writing
	{
		os.write((char*)&score, sizeof(int));
		os.write((char*)&lives, sizeof(int));
		os.close(); //Close score file after writing completed
	}
}

void Player::ScoreAndLivesFromFile(std::ifstream& is)
{
	int fileScore; //int to store score value
	int fileLives; //char buffer to store time data

	is.read((char*)&fileScore, sizeof(int)); //Read int
	is.read((char*)&fileLives, sizeof(int)); //read the time stamp stored as 20 byte char buffer

	score = fileScore;
	lives = fileLives;
}

