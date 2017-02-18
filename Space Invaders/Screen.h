#pragma once
#include "D3DGraphics.h"
#include <memory>

class ScreenContainer
{
	friend class Screen;
protected:
	std::unique_ptr<class Screen> pScreen;
};

class Screen
{
public:
	Screen(ScreenContainer* parent)
		:
		parent(parent),
		sscore(0)
	{}
	virtual void Update() = 0;
	virtual void Draw(D3DGraphics& gfx) = 0;

protected:
	void SetOtherParent(Screen& other) const
	{
		other.parent = parent;
	}
	void ChangeScreen(std::unique_ptr< Screen > pNewScreen)
	{
		parent->pScreen = std::move(pNewScreen);
	}
protected:
	ScreenContainer* parent;
	int sscore;
};