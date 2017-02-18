#pragma once
#include "D3DGraphics.h"
#include "Keyboard.h"
#include "Sound.h"
#include "Scoreboard.h"
#include "Starfield.h"
#include "Collision.h"
#include <vector>

class Level
{
public:
	Level(D3DGraphics& gfx, KeyboardClient& kbd, DSound& audio)
		:
		gfx(gfx),
		kbd(kbd),
		audio(audio),
		board("hiscore.txt"),
		p(audio, 386, 580),
		AABBposX(150.0f),
		AABBposY(100.0f),
		AABB(AABBposY, 190.0f, AABBposX, 650.0f),
		count(0),
		playerAlive(true)

	{
		for (int i = 0; i < 10; i++)
		{
			e.push_back(std::make_unique <Enemy>(L"Green\\Green", L"Green\\GreenDead.bmp", audio, (AABB.left + 9) + ((float)count1 * 50.0f), AABBposY + 5.0f, 2));
			count1++;
		}
		for (int i = 10; i < 20; i++)
		{
			e.push_back(std::make_unique <Enemy>(L"Pink\\Pink", L"Pink\\PinkDead.bmp", audio, (AABB.left + 9) + ((float)count2 * 50.0f), AABBposY + 35.0f, 2));
			count2++;
		}

		for (int i = 20; i < 30; i++)
		{
			e.push_back(std::make_unique <Enemy>(L"Blue\\Blue", L"Blue\\BlueDead.bmp", audio, (AABB.left + 9) + ((float)count3 * 50.0f), AABBposY + 65.0f, 2));
			count3++;
		}

	}
	~Level()
	{

	}
	bool IsAlive(Enemy* enemy)
	{
		if (enemy->IsAlive())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool AllEnemiesAlive()
	{
		for (auto i = e.begin(), end = e.end(); i != end; i++)
		{
			if ((*i)->IsAlive())
			{
				return true;
			}
		}
		return false;
	}
	void Update()
	{
		const KeyEvent key = kbd.ReadKey();
		p.Update(kbd);


		if (kbd.KeyIsPressed(VK_SPACE))
		{
			p.PlayLaserSound();
			p.FireBullet();
		}

		p.UpdateBullet();
		for (auto i = e.begin(), end = e.end(); i != end; i++)
		{
			(*i)->RandomFire();
			(*i)->UpdateBullet();
		}
		MoveAABBLeft();
		MoveAABBRight();
		MoveDown();
		for (auto i = e.begin(), end = e.end(); i != end; i++)
		{
			Collision::CheckForEnemyCollision(e, **i, p);
			Collision::CheckForPlayerCollision(**i, p);
			Collision::CheckForPlayerEnemyCollisionCollision(**i, p);
			Collision::CheckForEnemyHitBottom(**i, p);
			(*i)->Update();
		}

		starfield.ScrollField();
		if (p.IsPlayerDead())
		{
			playerAlive = false;
			board.AddScore(p.GetScore());
			board.SortVector();
			board.WriteScoresToFile("hiscore.txt", board.GetScoreboardVector());
		}
		else
		{
			if (!AllEnemiesAlive())
			{
				board.SortVector();
				board.WriteScoresToFile("hiscore.txt", board.GetScoreboardVector());
				p.ScoreAndLivesToFile();
			}
		}
	}

	void Draw(D3DGraphics& gfx)
	{
		for (auto i = e.begin(), end = e.end(); i != end; i++)
		{
			(*i)->GetDrawable().Rasterize(gfx);
		}
		starfield.Draw(gfx);
		p.Draw(gfx);
	}
	void MoveAABBRight()
	{
		if (AABB.right <= 750.0f && movingRight == true && movingDown == false)
		{
			AABB.left += AABBmovementSpeed;
			AABB.right += AABBmovementSpeed;
			for (auto i = e.begin(), end = e.end(); i != end; i++)
			{
				(*i)->MoveRight(AABBmovementSpeed);
			}
		}
		else if (AABB.right >= 750.0f && movingRight == true && movingDown == false)
		{
			movingDown = true;
		}
	}
	void MoveAABBLeft()
	{
		if (AABB.left >= 50.0f && movingRight == false && movingDown == false)
		{
			AABB.left -= AABBmovementSpeed;
			AABB.right -= AABBmovementSpeed;
			for (auto i = e.begin(), end = e.end(); i != end; i++)
			{
				(*i)->MoveLeft(AABBmovementSpeed);
			}
		}
		else if (AABB.left <= 50.0f && movingRight == false && movingDown == false)
		{
			movingDown = true;
		}
	}
	void MoveDown()
	{
		if (count < 25 && movingDown == true)
		{
			AABB.top += AABBmovementSpeed;
			AABB.bottom += AABBmovementSpeed;
			for (auto i = e.begin(), end = e.end(); i != end; i++)
			{
				(*i)->MoveDown(AABBmovementSpeed);
			}
			count++;
		}
		else
		{
			if (movingRight == true && movingDown == true)
			{
				movingRight = false;
				movingDown = false;
				count = 0;
			}
			else if (movingRight == false && movingDown == true)
			{
				movingRight = true;
				movingDown = false;
				count = 0;
			}
		}
	}
	void MakeRight()
	{
		movingRight = true;
	}
	void MakeLeft()
	{
		movingRight = false;
	}
	bool IsPlayerAlive()
	{
		return playerAlive;
	}
	Player& GetPlayer()
	{
		return p;
	}
private:
	D3DGraphics& gfx;
	KeyboardClient& kbd;
	DSound& audio;
	Player p;
	float AABBposX;
	float AABBposY;
	RectF AABB;
	static const int nEnemiesPerRow = 10;
	static const int nEnemiesPerCol = 3;
	std::vector<std::unique_ptr<Enemy>> e;
	Starfield starfield;
	Scoreboard board;
	int count;
	int count1 = 0;
	int count2 = 0;
	int count3 = 0;
	bool movingRight = true;
	bool movingLeft = false;
	bool movingDown = false;
	float AABBmovementSpeed = 0.5f;
	bool playerAlive;
};