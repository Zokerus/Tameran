#include "Rectangle.h"

Hydro::Rectangle::Rectangle()
	:Rectangle(0, 0, 0, 0)
{}

Hydro::Rectangle::Rectangle(int xPos, int yPos, int width, int height)
	: m_xPos(xPos), m_yPos(yPos), m_width(width), m_height(height)
{}

Hydro::Rectangle::~Rectangle()
{}

int Hydro::Rectangle::GetXPos() const
{
	return m_xPos;
}

int Hydro::Rectangle::GetYPos() const
{
	return m_yPos;
}

int Hydro::Rectangle::GetWidth() const
{
	return m_width;
}

int Hydro::Rectangle::GetHeight() const
{
	return m_height;
}

void Hydro::Rectangle::SetParameters(int xPos, int yPos, int width, int height)
{
	//Set the values of the rectangle
	m_xPos = xPos;
	m_yPos = yPos;
	m_width = width;
	m_height = height;
}