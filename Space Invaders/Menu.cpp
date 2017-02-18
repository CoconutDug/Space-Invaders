#include "Menu.h"

Menu::Button::Button(const Menu& menu, const std::wstring& text)
	:
	menu(menu),
	text(text),
	colorSelected(menu.selectedColor),
	colorDeselected(menu.deselectedColor),
	textColor(menu.textColor),
	buttonFont(menu.buttonFont),
	borders(menu.CalculateButtontemBorders())
{}
void Menu::Button::Draw(D3DGraphics& gfx)
{
	if (IsSelected())
	{
		gfx.DrawRectangle(borders, colorSelected);
	}
	else
	{
		gfx.DrawRectangle(borders, colorDeselected);
	}
	gfx.DrawString(text, borders, buttonFont, textColor);
}

void Menu::Button::HandleInput(const KeyEvent& e)
{
	if (e.GetCode() == VK_RETURN && e.IsPress())
	{
		OnPress();
	}
}

bool Menu::Button::IsSelected() const
{
	return selected;
}
