#include "Character.h"

Tameran::Character::Character()
{
}

bool Tameran::Character::Load(std::ifstream& in)
{
	return false;
}

const std::string Tameran::Character::Name() const
{
	return name;
}

Tameran::Character::CharacterType Tameran::Character::Type() const
{
	return type;
}

unsigned short Tameran::Character::Level() const
{
	return level;
}

unsigned int Tameran::Character::Experience() const
{
	return experience;
}

unsigned int Tameran::Character::SkillPoints() const
{
	return skillpoints;
}

bool Tameran::Character::Sex() const
{
	return sex;
}

unsigned short Tameran::Character::BaseHitPoints() const
{
	return baseHitPoints;
}

unsigned short Tameran::Character::BaseManaPoints() const
{
	return baseManapoints;
}

int Tameran::Character::CurrHitpoints() const
{
	return currentHitPoints;
}

int Tameran::Character::CurrManaPoints() const
{
	return currentManaPoints;
}

bool Tameran::Character::UnlockedMagic() const
{
	return unlockedMagic;
}

Tameran::CharacterStat Tameran::Character::Stat(int index) const
{
	return stats[index];
}
