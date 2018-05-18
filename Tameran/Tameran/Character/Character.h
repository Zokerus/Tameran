#ifndef CHARACTER
#define CHARACTER

//Includes
#include <string>
#include <vector>
#include <fstream>

//My Includes
#include "Stat/CharacterStat.h"
#include "Stat/BonusStat.h"
#include "Stat/Stat.h"

//////////////////////////////////////////////////////////////////////////
//Class: Character
//////////////////////////////////////////////////////////////////////////
namespace Tameran
{
	class Character
	{
	public:
		enum CharacterType { PC, NPC, Creature, Monster };

	public:
		Character();

		bool Load(std::ifstream& in);

		//Get
		const std::string Name() const;
		CharacterType Type() const;
		unsigned short Level() const;
		unsigned int Experience() const;
		unsigned int SkillPoints() const;
		bool Sex() const;
		unsigned short BaseHitPoints() const;
		unsigned short BaseManaPoints() const;
		int CurrHitpoints() const;
		int CurrManaPoints() const;
		bool UnlockedMagic() const;
		CharacterStat Stat(int index) const;

		//Set
		void AddExperience(int _exp);
		void AddSkillExperience(int _sexp);
		void TakeDamage(int _damage);
		void UseMana(int _mana);
		//Spells and buffs

	private:
		CharacterType type;
		std::string name;
		unsigned short level;
		unsigned int experience;
		unsigned int skillpoints;
		unsigned int skillexperience;
		unsigned char race; //we will see
		bool sex;
		unsigned char age; //later on

		unsigned short baseHitPoints;
		int currentHitPoints;
		unsigned short baseManapoints;
		int currentManaPoints;
		bool unlockedMagic;
		std::vector<CharacterStat> stats;

		bool isVendor;
		//picture
		//skilss
		//jobs
		//inventory
	};
}
#endif // !CHARACTER

