#pragma once
#include "Screen.h"
#include "Starfield.h"

class PreGameScreen : public Screen
{
public:
	class Drawable : public ::Drawable
	{
	public:
		Drawable(PreGameScreen& parent)
			:
			parent(parent)
		{}
		virtual void Rasterize(D3DGraphics& gfx) const
		{
			if (parent.count > 80)
			{
				gfx.DrawString(L"Loading.", { 230.0f, 250.0f }, parent.arialFont, WHITE);
				parent.count--;
			}
			else if (parent.count > 60)
			{
				gfx.DrawString(L"Loading..", { 230.0f, 250.0f }, parent.arialFont, WHITE);
				parent.count--;
			}
			else if (parent.count > 40)
			{
				gfx.DrawString(L"Loading...", { 230.0f, 250.0f }, parent.arialFont, WHITE);
				parent.count--;
			}
			else
			{
				gfx.DrawString(L"Loading...", { 230.0f, 250.0f }, parent.arialFont, WHITE);
				parent.count--;
			}
			parent.sField.Draw(gfx);
		}
	private:
		PreGameScreen& parent;
	};
public:
	PreGameScreen(std::unique_ptr< Screen > child, D3DGraphics& gfx, KeyboardClient& kbd, DSound& audio, ScreenContainer* ctr)
		:
		Screen(ctr),
		gfx(gfx),
		arialFont(L"Arial", 40),
		kbd(kbd),
		audio(audio),
		child(std::move(child)),
		count(100)
	{

	}

	virtual void Update() override
	{
		sField.ScrollField();
		if (count <= 0)
		{
			ChangeScreen(std::move(child));
		}
	}
	virtual void Draw(D3DGraphics& gfx) override
	{
		GetDrawable().Rasterize(gfx);
	}
	Drawable GetDrawable()
	{
		return Drawable(*this);
	}
private:
	D3DGraphics& gfx;
	KeyboardClient& kbd;
	DSound& audio;
	std::unique_ptr< Screen > child;
	Font arialFont;
	Starfield sField;
	int count;
};