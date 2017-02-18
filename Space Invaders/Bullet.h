#pragma once
#include "Surface.h"
#include "D3DGraphics.h"
#include "Drawable.h"
#include "Sound.h"

class Bullet
{
public:
	class Drawable : public ::Drawable
	{
	public:
		Drawable(Bullet& parent)
			:
			parent(parent)
		{}

		virtual void Rasterize(D3DGraphics& gfx) const
		{
			for (int y = 0; y < (int)parent.surface.GetHeight(); y++)
			{
				for (int x = 0; x < (int)parent.surface.GetWidth(); x++)
				{
					if (parent.key != parent.surface.GetPixel(x, y) && parent.fired == true)
					{
						gfx.PutPixel(x + parent.posX, y + parent.posY, parent.surface.GetPixel(x, y));
					}
				}
			}
			//parent.AABB.Draw(gfx);
		}
	private:
		Bullet& parent;
	};
public:
	Bullet(DSound& audio, int posX, int posY)
		:
		surface(Surface::FromFile(L"Bullet\\Bullet.bmp")),
		audio(audio),
		AABB(posY, posY + surface.GetHeight(), posX, posX + surface.GetWidth()),
		posX(posX),
		posY(posY),
		fired(false)
	{
		laser = audio.CreateSound("Laser.wav");
	}
	~Bullet()
	{}
	Drawable GetDrawable()
	{
		return Drawable(*this);
	}
	void UpdateBulletAABB()
	{
		AABB.top = posY;
		AABB.bottom = posY + surface.GetHeight();
		AABB.left = posX;
		AABB.right = posX + surface.GetWidth();
	}
	bool UpdatePlayerBullet(int newPosX, int newPosY)
	{
		if (fired == true)
		{
			posY -= 3;
			UpdateBulletAABB();
			if (AABB.top <= 0)
			{
				fired = false;
				SetPosX(newPosX);
				SetPosY(newPosY);
			}
		}
		
		return fired;
	}
	bool UpdateEnemyBullet(int newPosX, int newPosY)
	{
		if (fired == true)
		{
			posY += 3;
			UpdateBulletAABB();
			if (posY + surface.GetHeight() >= 599)
			{
				fired = false;
				SetPosX(newPosX);
				SetPosY(newPosY);
			}
		}
		return fired;
	}
	void BulletIsFired()
	{
		fired = true;
	}
	void PlayLaserSound()
	{
		laser.Play();
	}
	RectI GetAABB()
	{
		return AABB;
	}
	void SetPosX(int newPosX)
	{
		posX = newPosX;
	}
	void SetPosY(int newPosY)
	{
		posY = newPosY;
	}
	void SetNotFired()
	{
		fired = false;
	}
	bool BulletFired()
	{
		return fired;
	}
private:
	DSound& audio;
	Sound laser;
	Surface surface;
	RectI AABB;
	Color key = WHITE;
	int posX;
	int posY;
	bool fired;
};