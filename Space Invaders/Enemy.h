#pragma once
#include "Surface.h"
#include "D3DGraphics.h"
#include "Drawable.h"
#include "Rect.h"
#include "Bullet.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>

class Enemy
{
public:
	class Drawable : public ::Drawable
	{
	public:
		Drawable(Enemy& parent)
			:
			parent(parent)
		{}
		
		virtual void Rasterize(D3DGraphics& gfx) const
		{
			if (parent.alive == true)
			{				
				for (unsigned int y = 0; y < parent.pos->GetHeight(); y++)
				{
					for (unsigned int x = 0; x < parent.pos->GetWidth(); x++)
					{
						if (parent.key != parent.pos->GetPixel(x, y))
						{
							gfx.PutPixel(x + (int)parent.posX, y + (int)parent.posY, parent.pos->GetPixel(x, y));
						}
					}
				}
				
				//parent.AABB.Draw(gfx);
			}
			else if (parent.alive == false && parent.c >= 0)
			{
				for (int y = 0; y < (int)parent.surface.GetHeight(); y++)
				{
					for (int x = 0; x < (int)parent.surface.GetWidth(); x++)
					{
						if (parent.key != parent.surface.GetPixel(x, y))
						{
							gfx.PutPixel(x + (int)parent.posX, y + (int)parent.posY, parent.surface.GetPixel(x, y));
							
						}
					}
				}
				parent.c--;
			}
			if (parent.bulletFired)
			{
				parent.bullet->GetDrawable().Rasterize(gfx);
			}
		}
	private:
		Enemy& parent;
	};
public:
	Enemy(std::wstring filename, std::wstring dFilename, DSound& audio, float posX, float posY, int nSurfaces)
		:
		vSurface(Surface::VectorFromFile(filename, nSurfaces)),
		surface(Surface::FromFile(dFilename)),
		bullet(new Bullet(audio, (int)posX + 12, (int)posY)),
		pos(vSurface.begin()),
		AABB(posY, posY + pos->GetHeight(), posX, posX + pos->GetWidth()),
		posX(posX),
		posY(posY),
		bulletFired(false),
		alive(true),
		nSurfaces(nSurfaces)
	{
		srand((unsigned int)time(NULL));
		laser = audio.CreateSound("Laser.wav");
		explosion = audio.CreateSound("Explosion.wav");
	}
	Drawable GetDrawable()
	{
		return Drawable(*this);
	}
	void Update()
	{
		Advance();
	}
	void Advance()
	{
		curHoldCount++;
		if (curHoldCount >= nHoldFrames)
		{
			curHoldCount = 0;
			if (pos == vSurface.begin())
			{
				pos = vSurface.end() - 1;
			}
			else if (pos == vSurface.end() - 1)
			{
				pos = vSurface.begin();
			}
		}
	}
	void MoveRight(float movementSpeed)
	{
		posX += movementSpeed;
		AABB.left = posX;
		AABB.right = posX + vSurface.front().GetWidth();
	}
	void MoveLeft(float movementSpeed)
	{
		posX -= movementSpeed;
		AABB.left = posX;
		AABB.right = posX + vSurface.front().GetWidth();	
	}
	void MoveDown(float movementSpeed)
	{
		posY += movementSpeed;
		AABB.top = posY;
		AABB.bottom = posY + vSurface.front().GetHeight();
	}
	void Move(float movementSpeed)
	{
	
	}
	void RandomFire()
	{
		int i = rand() % 1000 + 1;
		if (i == 100 && alive)
		{
			FireBullet();
		}
	}
	void FireBullet()
	{
		if (bulletFired == false)
		{
			CreateBullet();
			UpdateBullet();
			bulletFired = true;
		}
	}
	void CreateBullet()
	{
		laser.Play();
		bullet->SetPosX((int)posX + 12);
		bullet->SetPosY((int)posY);
		bullet->BulletIsFired();
	}
	void UpdateBullet()
	{
		if (bulletFired == true)
		{
			if (!bullet->UpdateEnemyBullet((int)posX + 12, (int)posY))
			{
				bulletFired = false;
			}
		}
	}
	RectF GetAABB()
	{
		return AABB;
	}
	void SetDead()
	{
		explosion.Play();
		alive = false;
	}
	Bullet* GetEnemyBullet()
	{
		return bullet;
	}
	bool IsAlive()
	{
		return alive;
	}
	void IncreaseSpeed()
	{
		//movementSpeed += 0.03f;
	}
private:
	Sound laser;
	Sound explosion;
	std::vector<Surface> vSurface;
	std::vector<Surface>::iterator pos;
	Surface surface;
	Bullet* bullet;
	RectF AABB;
	bool bulletFired;
	float posX;
	float posY;
	int key = WHITE;
	int nSurfaces;
	bool alive;
	bool moveRight = true;
	int curHoldCount = 0;
	int nHoldFrames = 20;
	int iCurSurface = 0;
	int c = 3;
};