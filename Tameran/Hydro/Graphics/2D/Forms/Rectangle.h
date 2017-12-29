#ifndef RECTANGLE
#define RECTANGLE

//////////////////////////////////////////////////////////////////////////
//Class: Rectangle
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class Rectangle
	{
	public:
		Rectangle();
		Rectangle(int xPos, int yPos, int width, int height);
		~Rectangle();

		int GetXPos() const;
		int GetYPos() const;
		int GetWidth() const;
		int GetHeight() const;

		void SetParameters(int xPos, int yPos, int width, int height);

	private:
		int m_xPos, m_yPos, m_width, m_height;
	};
}

#endif // !RECTANGLE
