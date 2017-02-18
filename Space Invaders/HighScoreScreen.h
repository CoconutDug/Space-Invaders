#pragma once
#include "Screen.h"
#include "Surface.h"
#include "Keyboard.h"
#include "GameScreen.h"
#include "ChiliMath.h"
#include <random>
#include <vector>
#include <memory>

class HighScoreScreen : public Screen, public ScreenContainer
{
public:
	HighScoreScreen(std::unique_ptr< Screen > child, ScreenContainer* ctr, KeyboardClient& kbd)
		:
		Screen(ctr),
		kbd(kbd),
		child(std::move(child))
	{
		SetOtherParent(*this->child);
	}
	virtual void Draw(D3DGraphics& gfx) override
	{
		
	}
	virtual void Update(float x) override
	{
		const KeyEvent key = kbd.ReadKey();

		if (key.GetCode() == VK_ESCAPE && key.IsPress())
		{
			SetOtherParent(*child);
			ChangeScreen(std::move(child));
		}
	}
private:
	float time = 3.999f;
	std::unique_ptr< Screen > child;
	std::unique_ptr<Surface> surface;
	KeyboardClient& kbd;
};