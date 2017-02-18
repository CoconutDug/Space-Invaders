#pragma once
#include "Drawable.h"
#include "Screen.h"
#include "Keyboard.h"
#include "PreGameScreen.h"
#include "GameScreen.h"
#include "HighScoresScreen.h"
#include "Starfield.h"
#include "Menu.h"

class TitleScreen : public Screen
{
private:
	class StartMenu : public Menu
	{
	public:
		StartMenu(TitleScreen& parent)
			:
			Menu({ 400, 350 }, 200, 17, 7, L"Arial", { 128, 128, 128 },
			{52, 85, 101}, {52, 28, 120}, 3),
			parent(parent)
		{
			AddItem(std::make_unique<StartButton>(*this));
			AddItem(std::make_unique<HighScoreButton>(*this));
			AddItem(std::make_unique<QuitGameButton>(*this));
			Finalize();
		}
	private:
		TitleScreen& parent;
	private:
		class StartButton : public Button
		{
		public:
			StartButton(const StartMenu& menu)
				:
				Button(menu, L"Start"),
				screen(menu.parent)
			{}
			void OnPress() override
			{
				screen.StartGame();
			}
		private:
			TitleScreen& screen;
		};
	private:
		class HighScoreButton : public Button
		{
		public:
			HighScoreButton(const StartMenu& menu)
				:
				Button(menu, L"High Scores"),
				screen(menu.parent)
			{}
			void OnPress() override
			{
				screen.GoToHiScores();
			}
		private:
			TitleScreen& screen;
		};
	private:
		class QuitGameButton : public Button
		{
		public:
			QuitGameButton(const StartMenu& menu)
				:
				Button(menu, L"Quit Game"),
				screen(menu.parent)
			{}
			void OnPress() override
			{
				exit(EXIT_SUCCESS);
			}
		private:
			TitleScreen& screen;
		};
	};
public:
	class Drawable : public ::Drawable
	{
	public:
		Drawable(TitleScreen& parent)
			:
			parent(parent)
		{}
		virtual void Rasterize(D3DGraphics& gfx) const
		{
			for (int y = 0; y < (int)parent.surface.GetHeight(); y++)
			{
				for (int x = 0; x < (int)parent.surface.GetWidth(); x++)
				{
					gfx.PutPixel(x + 182, y + 100, parent.surface.GetPixel(x, y));
				}
			}
			parent.sField.Draw(gfx);
			parent.pMenu->DrawMenu(gfx);
		}
	private:
		TitleScreen& parent;
	};
public:
	TitleScreen(D3DGraphics& gfx, KeyboardClient& kbd, DSound& audio, ScreenContainer* ctr)
		:
		Screen(ctr),
		kbd(kbd),
		gfx(gfx),
		audio(audio),
		arialFont(L"Arial", 10),
		surface(Surface::FromFile(L"Logo.bmp"))
	{
		pMenu = std::make_unique<StartMenu>(*this);
	}
	~TitleScreen()
	{
		
	}
	virtual void Draw(D3DGraphics& gfx) override
	{
		GetDrawable().Rasterize(gfx);
	}
	virtual void Update() override
	{
		sField.ScrollField();
		const KeyEvent key = kbd.ReadKey();
		if (pMenu)
		{
			pMenu->HandleInput(key);
		}
	}
	Drawable GetDrawable()
	{
		return Drawable(*this);
	}
	void StartGame()
	{
		ChangeScreen(std::make_unique< PreGameScreen >(std::make_unique< GameScreen >(std::make_unique< TitleScreen >(gfx, kbd, audio, parent), gfx, kbd, audio, parent), gfx, kbd, audio, parent));
	}
	void GoToHiScores()
	{
		ChangeScreen(std::make_unique< HighScoresScreen >(std::make_unique< TitleScreen >(gfx, kbd, audio, parent), gfx, kbd, audio, parent));
	}
private:
	D3DGraphics& gfx;
	KeyboardClient& kbd;
	DSound& audio;
	Font arialFont;
	Surface surface;
	Starfield sField;
	std::unique_ptr<StartMenu> pMenu;
};