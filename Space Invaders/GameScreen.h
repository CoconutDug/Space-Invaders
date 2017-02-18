#pragma once
#include "Screen.h"
#include "Keyboard.h"
#include "Sound.h"
#include "Scoreboard.h"
#include "Level.h"
#include "Menu.h"
#include <vector>

class GameScreen : public Screen
{
private:
	class LoseMenu : public Menu
	{
	public:
		LoseMenu(GameScreen& parent)
			:
			Menu({ 400, 400 }, 200, 17, 7, L"Arial", { 128, 128, 128 },
			{ 52, 85, 101 }, { 52, 28, 120 }, 2),
			parent(parent)
		{
			AddItem(std::make_unique<NewGameButton>(*this));
			AddItem(std::make_unique<MainMenuButton>(*this));
			Finalize();
		}
	private:
		GameScreen& parent;
	private:
		class NewGameButton : public Button
		{
		public:
			NewGameButton(const LoseMenu& menu)
				:
				Button(menu, L"New Game"),
				screen(menu.parent)
			{}
			void OnPress() override
			{
				screen.currentLevel = std::make_unique<Level>(screen.gfx, screen.kbd, screen.audio);
			}
		private:
			GameScreen& screen;
		};
	private:
		class MainMenuButton : public Button
		{
		public:
			MainMenuButton(const LoseMenu& menu)
				:
				Button(menu, L"Main Menu"),
				screen(menu.parent)
			{}
			void OnPress() override
			{
				screen.currentLevel->GetPlayer().ResetScoreAndLives();
				screen.ChangeScreen(std::move(screen.child));
			}
		private:
			GameScreen& screen;
		};
	};
	class WinMenu : public Menu
	{
	public:
		WinMenu(GameScreen& parent)
			:
			Menu({ 400, 400 }, 200, 17, 7, L"Arial", { 128, 128, 128 },
			{ 52, 85, 101 }, { 52, 28, 120 }, 2),
			parent(parent)
		{
			AddItem(std::make_unique<NextLevelButton>(*this));
			AddItem(std::make_unique<MainMenuButton>(*this));
			Finalize();
		}
	private:
		GameScreen& parent;
	private:
		class NextLevelButton : public Button
		{
		public:
			NextLevelButton(const WinMenu& menu)
				:
				Button(menu, L"Next Level"),
				screen(menu.parent)
			{}
			void OnPress() override
			{
				screen.currentLevel = std::make_unique<Level>(screen.gfx, screen.kbd, screen.audio);
			}
		private:
			GameScreen& screen;
		};
	private:
		class MainMenuButton : public Button
		{
		public:
			MainMenuButton(const WinMenu& menu)
				:
				Button(menu, L"Main Menu"),
				screen(menu.parent)
			{}
			void OnPress() override
			{
				screen.currentLevel->GetPlayer().ResetScoreAndLives();
				screen.ChangeScreen(std::move(screen.child));
			}
		private:
			GameScreen& screen;
		};
	};
public:
	GameScreen(std::unique_ptr< Screen > child, D3DGraphics& gfx, KeyboardClient& kbd, DSound& audio, ScreenContainer* ctr)
		:
		Screen(ctr),
		gfx(gfx),
		kbd(kbd),
		audio(audio),
		gameWon(Surface::FromFile(L"YouWin.bmp")),
		gameOver(Surface::FromFile(L"GameOver.bmp")),
		child(std::move(child)),
		arialFont(L"Arial", 10)
	{
		pLoseMenu = std::make_unique<LoseMenu>(*this);
		pWinMenu = std::make_unique<WinMenu>(*this);
		currentLevel = std::make_unique<Level>(gfx, kbd, audio);
	}
	~GameScreen()
	{
	
	}
	virtual void Update() override
	{	
		const KeyEvent key = kbd.ReadKey();
		if (currentLevel->AllEnemiesAlive() && currentLevel->IsPlayerAlive())
		{
			currentLevel->Update();
		}
		else
		{
			if (!currentLevel->AllEnemiesAlive())
			{
				pWinMenu->HandleInput(key);
			}
			else if (!currentLevel->IsPlayerAlive())
			{
				pLoseMenu->HandleInput(key);
			}		
		}
	}
	virtual void Draw(D3DGraphics& gfx) override
	{
		currentLevel->Draw(gfx);
		if (!currentLevel->AllEnemiesAlive())
		{
			gfx.DrawKeyedSurface(gameWon, 281, 200, BLACK);
			pWinMenu->DrawMenu(gfx);
		}
		else if (!currentLevel->IsPlayerAlive())
		{
			gfx.DrawKeyedSurface(gameOver, 281, 100, BLACK);
			pLoseMenu->DrawMenu(gfx);
		}

		gfx.DrawString(L"Lives: ", { 10.0f, 10.0f }, arialFont, WHITE);
		gfx.DrawString(std::to_wstring(int(currentLevel->GetPlayer().GetLives())), { 50.0f, 10.0f }, arialFont, WHITE);

		gfx.DrawString(L"Score: ", { 720.0f, 10.0f }, arialFont, WHITE);
		gfx.DrawString(std::to_wstring(int(currentLevel->GetPlayer().GetScore())), { 770.0f, 10.0f }, arialFont, WHITE);
	}
private:
	D3DGraphics& gfx;
	KeyboardClient& kbd;
	DSound& audio;
	Surface gameWon;
	Surface gameOver;
	std::unique_ptr< Screen > child;
	std::unique_ptr<Level> currentLevel;
	std::unique_ptr<LoseMenu> pLoseMenu;
	std::unique_ptr<WinMenu> pWinMenu;
	Font arialFont;
};