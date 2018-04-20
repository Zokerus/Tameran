#include "Rectangle.h"

Hydro::Rectangle::Rectangle()
	:Rectangle(0, 0, 0, 0)
{}

Hydro::Rectangle::Rectangle(int XPos, int YPos, int Width, int Height)
	: xPos(XPos), yPos(YPos), width(Width), height(Height)
{}

Hydro::Rectangle::~Rectangle()
{}

int Hydro::Rectangle::GetXPos() const
{
	return xPos;
}

int Hydro::Rectangle::GetYPos() const
{
	return yPos;
}

int Hydro::Rectangle::GetWidth() const
{
	return width;
}

int Hydro::Rectangle::GetHeight() const
{
	return height;
}

void Hydro::Rectangle::SetParameters(int XPos, int YPos, int Width, int Height)
{
	//Set the values of the rectangle
	xPos = XPos;
	yPos = YPos;
	width = Width;
	height = Height;
}