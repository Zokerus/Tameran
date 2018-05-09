#include "CharacterStat.h"

Tameran::CharacterStat::CharacterStat(std::string _Stat, int _value)
	: stat(_Stat), value(_value), bonuses()
{}

const std::string Tameran::CharacterStat::StatName() const
{
	return stat;
}

const int Tameran::CharacterStat::Value() const
{
	int currValue = value;

	for(int i = 0; i < bonuses.size(); i++)
	{
		currValue = currValue + bonuses[i].Value();
	}

	return currValue;
}

const int Tameran::CharacterStat::BaseValue() const
{
	return value;
}

const std::vector<Tameran::BonusStat> Tameran::CharacterStat::Bonuses()
{
	return bonuses;
}

void Tameran::CharacterStat::StatName(std::string name)
{
	if (!name.empty())
	{
		stat = name;
	}
}

void Tameran::CharacterStat::BaseValue(int _value)
{
	if (_value >= 1)
	{
		value = _value;
	}
}

void Tameran::CharacterStat::Bonuses(std::vector<BonusStat> _bonuses)
{
	bonuses.clear();

	bonuses = _bonuses;
}

void Tameran::CharacterStat::IncreaseValue(int _value)
{
	value = value + _value;
}

void Tameran::CharacterStat::ReduceValue(int _value)
{
	value = value - _value;
	if (value < 1)
	{
		value = 1;
	}
}

void Tameran::CharacterStat::AddBonus(BonusStat _bonus)
{
	bonuses.emplace_back(_bonus);
}

void Tameran::CharacterStat::Update(float eTime)
{
	//looping through the vector by using iterators
	for (std::vector<BonusStat>::iterator bonus = bonuses.begin(); bonus != bonuses.end(); bonus++)
	{
		if (bonus->Timed())
		{
			//Calculating remaining time of the bonus
			bonus->Duration(bonus->Duration() - eTime);
			if (bonus->Duration() < 0.0f)
			{
				//time is up and the bonus will be removed
				bonuses.erase(bonus);
			}
		}
	}
}
