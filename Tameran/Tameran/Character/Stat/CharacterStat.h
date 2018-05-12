#ifndef CHARACTERSTAT
#define CHARACTERSTAT

#include <string>
#include <vector>

//My Include
#include "BonusStat.h"


//////////////////////////////////////////////////////////////////////////
//Class: CharacterStat
//////////////////////////////////////////////////////////////////////////
namespace Tameran
{
	class CharacterStat
	{
	public:
		CharacterStat(std::string _Stat, int _value);

		//Get
		const std::string StatName() const;
		const int Value() const;
		const int BaseValue() const;
		const std::vector<BonusStat> Bonuses();

		//Set
		void StatName(std::string name);
		void BaseValue(int _value);
		void Bonuses(std::vector<BonusStat> _bonuses);

		void IncreaseValue(int _value);
		void ReduceValue(int _value);
		void AddBonus(BonusStat _bonus);
		//void DeleteBonus();

		void Update(float eTime);
	private:
		std::string stat;
		int value;
		std::vector<BonusStat> bonuses;
	};
}
#endif // !CHARACTERSTAT


