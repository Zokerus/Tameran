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
		Rectangle(int XPos, int YPos, int Width, int Height);
		~Rectangle();

		int GetXPos() const;
		int GetYPos() const;
		int GetWidth() const;
		int GetHeight() const;

		void SetParameters(int XPos, int YPos, int Width, int Height);

	private:
		int xPos, yPos, width, height;
	};
}

#endif // !RECTANGLE
