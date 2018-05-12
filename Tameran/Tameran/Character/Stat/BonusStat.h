#ifndef BONUSSTAT
#define BONUSSTAT

//////////////////////////////////////////////////////////////////////////
//Class: BonusStat
//Description: Boni on Stats from characters or items. They are stored 
//seperatly and the currect Stat is calculated.
//////////////////////////////////////////////////////////////////////////
namespace Tameran
{
	class BonusStat
	{
	public:
		BonusStat(int _value, int _timeStarted, float _duration, bool _timed);

		//Get
		const int Value() const;
		const int StartingTime() const;
		const float Duration();
		const bool Timed() const;

		//Set
		void Value(int _value);
		void StartingTime(int _timeStarted);
		void Duration(float _duration);
		void Timed(bool _timed);

	private:
		int value;
		int timeStarted;
		float duration;
		bool timed;
	};
}
#endif // !BONUSSTAT

