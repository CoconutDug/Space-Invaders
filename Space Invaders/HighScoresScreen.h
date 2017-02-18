#pragma once
#include "Screen.h"
#include "Keyboard.h"
#include "Scoreboard.h"
#include "Menu.h"

class HighScoresScreen : public Screen
{
private:
	class HighScoreMenu : public Menu
	{
	public:
		HighScoreMenu(HighScoresScreen& parent)
			:
			Menu({ 400, 500 }, 200, 17, 7, L"Arial", { 128, 128, 128 },
			{ 52, 85, 101 }, { 52, 28, 120 }, 1),
			parent(parent)
		{
			AddItem(std::make_unique<BackButton>(*this));
			Finalize();
		}
	private:
		HighScoresScreen& parent;
	private:
		class BackButton : public Button
		{
		public:
			BackButton(const HighScoreMenu& menu)
				:
				Button(menu, L"Main Menu"),
				screen(menu.parent)
			{}
			void OnPress() override
			{
				screen.BackToTitleScreen();
			}
		private:
			HighScoresScreen& screen;
		};
	};
public:
	HighScoresScreen(std::unique_ptr< Screen > child, D3DGraphics& gfx, KeyboardClient& kbd, DSound& audio, ScreenContainer* ctr)
		:
		Screen(ctr),
		gfx(gfx),
		kbd(kbd),
		audio(audio),
		child(std::move(child)),
		arialFont(L"Arial", 10),
		arialFontBig(L"Arial", 30),
		board("hiscore.txt")
	{
		pMenu = std::make_unique<HighScoreMenu>(*this);
	}
	~HighScoresScreen()
	{
		
	}
	virtual void Update() override
	{
		starfield.ScrollField();
		const KeyEvent key = kbd.ReadKey();
		if (pMenu)
		{
			pMenu->HandleInput(key);
		}
	}
	virtual void Draw(D3DGraphics& gfx) override
	{
		starfield.Draw(gfx);
		gfx.DrawString(L"HIGH SCORES", { 230.0f, 80.0f }, arialFontBig, WHITE);
		gfx.DrawString(L"Position", { 230.0f, 150.0f }, arialFont, WHITE);
		gfx.DrawString(L"Score", { 335.0f, 150.0f }, arialFont, WHITE); 
		gfx.DrawString(L"Time", { 495.0f, 150.0f }, arialFont, WHITE);

		for (int i = 0; i < board.GetnScores() && board.GetnScores() <= 10; i++)
		{
			gfx.DrawString(board.PrintScore(i), { 230.0f, 170.0f + (i * 32) }, arialFont, WHITE);
		}
		pMenu->DrawMenu(gfx);
	}
	void BackToTitleScreen()
	{
		ChangeScreen(std::move(child));
	}
private:
	D3DGraphics& gfx;
	KeyboardClient& kbd;
	DSound& audio;
	Starfield starfield;
	std::unique_ptr< Screen > child;
	Scoreboard board;
	Font arialFont;
	Font arialFontBig;
	std::unique_ptr<HighScoreMenu> pMenu;
};