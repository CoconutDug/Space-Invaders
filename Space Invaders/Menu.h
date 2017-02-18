#pragma once
#include "D3DGraphics.h"
#include "Keyboard.h"
#include <memory>

class Menu
{
protected:
	class Button
	{
		friend Menu;
	public:
		Button(const Menu& menu, const std::wstring& text);
		void Update(float);
		void Draw(D3DGraphics& gfx);
		void HandleInput(const KeyEvent& e);
		virtual void OnPress() = 0;
		bool IsSelected() const;
		RectI CalculateButtonBorders();
		
	protected:
		const Menu& menu;
		std::wstring text;
		Color colorSelected;
		Color colorDeselected;
		Color textColor;
		bool selected = false;
		const Font& buttonFont;
		const RectI borders;
	};
public:
	Menu(Vei2 centre, const int buttonWidth, const int buttonHeight, const int padding,
		const std::wstring& text, Color textColor, Color selectedColor,
		Color deselectedColor, int nButtons = 0)
		:
		centre(centre),
		borders(CalculateMenuBorders(centre, nButtons, buttonWidth, buttonHeight, padding)),
		padding(padding),
		buttonWidth(buttonWidth),
		buttonHeight(buttonHeight),
		text(text),
		textColor(textColor),
		buttonFont(text, 11),
		selectedColor(selectedColor),
		deselectedColor(deselectedColor)
	{
		buttons.reserve(nButtons);
	}
	RectI CalculateButtontemBorders() const
	{
		int i = buttons.size();
		return{
			borders.top + padding + (buttonHeight + padding) * i,
			borders.top + (buttonHeight + padding) * (i + 1),
			borders.left + padding,
			borders.right - padding
		};
	}
	static RectI CalculateMenuBorders(Vei2 center, int nButtons, int buttonWidth, int buttonHeight, int padding)
	{
		const int halfHeight = ((buttonHeight + padding) * nButtons + padding) / 2;
		const int halfWidth = (buttonWidth / 2) + padding;
		return{
			center.y - halfHeight,
			center.y + halfHeight,
			center.x - halfWidth,
			center.x + halfWidth
		};
	}
	void AddItem(std::unique_ptr<Button> button)
	{
		buttons.emplace_back(std::move(button));
	}
	void Finalize()
	{
		pos = buttons.begin();
		buttons.front()->selected = true;
	}
		
	void DrawMenu(D3DGraphics& gfx)
	{
		gfx.DrawRectangleAlpha(borders, { 125, 247, 247, 27 });
		for (const auto& pButton : buttons)
		{
			pButton->Draw(gfx);
		}
	}
	void HandleInput(const KeyEvent& e)
	{
		if (e.IsPress())
		{
			if (e.GetCode() == VK_UP)
			{
				(*pos)->selected = false;
				if (pos == buttons.begin())
				{
					pos = buttons.end() - 1;
				}
				else
				{
					pos--;
				}
				(*pos)->selected = true;
				return;
			}
			else if (e.GetCode() == VK_DOWN)
			{
				(*pos)->selected = false;
				pos++;
				if (pos == buttons.end())
				{
					pos = buttons.begin();
				}
				(*pos)->selected = true;
				return;
			}
		}
		(*pos)->HandleInput(e);
	}
private:
	std::vector<std::unique_ptr<Button>>::iterator pos;
	std::vector<std::unique_ptr<Button>> buttons;
	const Vei2 centre;
	const RectI borders;
	const int textPadding = 0;
	const int padding;
	const int buttonWidth;
	const int buttonHeight;
	const std::wstring& text;
	const Font buttonFont;
	const Color textColor;
	const Color selectedColor;
	const Color deselectedColor;
	int nButtons;
};