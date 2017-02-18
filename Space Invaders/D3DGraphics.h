/******************************************************************************************
*	Chili DirectX Framework Version 14.03.22											  *
*	D3DGraphics.h																		  *
*	Copyright 2014 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include <d3d9.h>
#include "GdiPlusManager.h"
#include "Vec2.h"
#include "Colors.h"
#include "Vertex.h"
#include "DrawTarget.h"
#include "Surface.h"

class D3DGraphics : public DrawTarget
{
public:
	D3DGraphics(HWND hWnd);
	~D3DGraphics();
	void PutPixel(int x, int y, Color c);
	void PutPixelAlpha(int x, int y, Color c);
	Color GetPixel(int x, int y) const;
	void DrawSurface(Color key, int posX, int posY);
	template< typename T >
	inline void DrawRectangle(const Rect<T>& rect, Color c)
	{
		DrawRectangle((int)rect.left, (int)rect.right, (int)rect.top, (int)rect.bottom, c);
	}
	template< typename T >
	inline void DrawRectangle(_Vec2< T > topLeft, _Vec2< T > bottomRight, Color c)
	{
		DrawRectangle((int)topLeft.x, (int)bottomRight.x, (int)topLeft.y, (int)bottomRight.y, c);
	}
	//void DrawRectangle(int left, int right, int top, int bottom, Color c);
	template< typename T >
	inline void DrawLine(_Vec2< T > pt1, _Vec2< T > pt2, Color c)
	{
		DrawLine((int)pt1.x, (int)pt1.y, (int)pt2.x, (int)pt2.y, c);
	}
	template< typename T >
	inline void DrawRectangleAlpha(const Rect<T>& rect, Color c)
	{
		DrawRectangle((int)rect.left, (int)rect.right, (int)rect.top, (int)rect.bottom, c,
			&D3DGraphics::PutPixelAlpha);
	}
	void DrawRectangle(int left, int right, int top, int bottom, Color c, void(D3DGraphics::*)(int, int, Color) = &D3DGraphics::PutPixel);
	void DrawLine(int x1, int y1, int x2, int y2, Color c);
	void DrawLineClip(Vec2 p0, Vec2 p1, Color color, const RectF& clip);
	template< typename T >
	inline void DrawCircle(_Vec2< T > center, int radius, Color c)
	{
		DrawCircle((int)center.x, (int)center.y, radius, c);
	}
	void DrawCircle(int centerX, int centerY, int radius, Color c);
	inline void DrawString(const std::wstring& string, Vec2 pt, const Font& font, Color c = WHITE)
	{
		sysBuffer.DrawString(string, pt, font, c);
	}
	inline void DrawString2(const std::wstring& string, Vec2 pt, const Font& font, Color c = WHITE)
	{
		sysBuffer.DrawString(string, pt, font, c);
	}
	inline void DrawString(const std::wstring& string, const RectF &rect, const Font& font, Color c = WHITE, Font::Alignment a = Font::Center)
	{
		sysBuffer.DrawString(string, rect, font, c, a);
	}
	inline void CopySurface(const Surface& src)
	{
		sysBuffer.Copy(src);
	}
	inline void DrawKeyedSurface(Surface& surface, int posX, int posY, D3DCOLOR key)
	{
		for (int y = 0; y < (int)surface.GetHeight(); y++)
		{
			for (int x = 0; x < (int)surface.GetWidth(); x++)
			{
				if (key != surface.GetPixel(x, y))
				{
					PutPixel(x + posX, y + posY, surface.GetPixel(x, y));
				}
			}
		}
	}
	void BeginFrame();
	void EndFrame();
	virtual void Draw(Drawable& obj) override
	{
		obj.Rasterize(*this);
	}
	void DrawTriangle(Vec2 v0, Vec2 v1, Vec2 v2, const RectI& clip, Color c);
	void DrawTriangleTex(Vertex v0, Vertex v1, Vertex v2, const RectI& clip, const Surface &tex);
public:
	static const unsigned int	SCREENWIDTH = 800;
	static const unsigned int	SCREENHEIGHT = 600;
private:
	void DrawFlatTopTriangleTex(Vertex v0, Vertex v1, Vertex v2, const RectI& clip, const Surface &tex);
	void DrawFlatBottomTriangleTex(Vertex v0, Vertex v1, Vertex v2, const RectI& clip, const Surface &tex);
	void DrawFlatTriangle(float yStart, float yEnd, float m0, float b0, float m1, float b1, const RectI& clip, Color c);
private:
	GdiPlusManager		gdiManager;
	const Color			FILLVALUE = BLACK;
	IDirect3D9*			pDirect3D;
	IDirect3DDevice9*	pDevice;
	IDirect3DSurface9*	pBackBuffer;
	TextSurface			sysBuffer;
};