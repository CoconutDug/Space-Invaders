#pragma once
#include "D3DGraphics.h"
#include "Keyboard.h"
#include "Bullet.h"
#include "Rect.h"
#include <fstream>

class Player
{
public:
	Player(DSound& audio, int posX, int posY);
	~Player();
	void Draw(D3DGraphics& gfx);
	void Update(KeyboardClient& kbd);
	void MoveLeft();
	void MoveRight();
	void FireBullet();
	void CreateBullet();
	void UpdateBullet();
	int& GetScore();
	int& GetLives();
	Bullet* GetPlayerBullet();
	RectI GetAABB();
	void IncreaseScore();
	void PlayerHit();
	void PlayerNotHit();
	void LoseLife();
	bool IsPlayerHit();
	void ResetPlayer();
	void PlayLaserSound();
	void ResetScoreAndLives();
	bool IsPlayerDead();
	void ScoreAndLivesToFile();
	void ScoreAndLivesFromFile(std::ifstream& is);
private:
	Surface aSurface;
	Bullet* bullet;
	bool bulletFired;
	Color key;
	Sound explosion;
	int posX;
	int posY;
	RectI AABB;
	int score;
	int lives;
	bool playerHit;
	bool playerAlive;
};