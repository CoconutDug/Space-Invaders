#pragma once
#include "D3DGraphics.h"
#include <stdlib.h>
#include <time.h>

class Starfield
{
public:
	class Star
	{
	public:
		Star(float posX, float posY)
			:
			posX(posX),
			posY(posY)
		{

		}
		void Draw(D3DGraphics& gfx)
		{
			gfx.PutPixel((int)posX, (int)posY, colour);
		}
		void ScrollStar()
		{
			posY += 0.5f;
			if (posY >= 599)
			{
				posY = 0;
			}
		}
	private:
		float posX;
		float posY;
		D3DCOLOR colour = WHITE;
	};
public:
	Starfield()
	{
		srand((unsigned int)time(NULL));
		for (int i = 0; i < nStars; i++)
		{
			float posX = (float)(rand() % 800);
			float posY = (float)(rand() % 600);
			star[i] = new Star(posX, posY);
		}
	}
	~Starfield()
	{
		for (int i = 0; i < nStars; i++)
		{
			delete star[i];
		}
	}
	void Draw(D3DGraphics& gfx)
	{
		for (int i = 0; i < nStars; i++)
		{
			star[i]->Draw(gfx);
		}
	}
	void ScrollField()
	{
		for (int i = 0; i < nStars; i++)
		{
			star[i]->ScrollStar();
		}
	}
private:
	static const int nStars = 100;
	Star* star[nStars];
};