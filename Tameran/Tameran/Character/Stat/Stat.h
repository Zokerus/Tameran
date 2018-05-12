#ifndef STAT
#define STAT

#include <string>

//////////////////////////////////////////////////////////////////////////
//Class: Stat
//////////////////////////////////////////////////////////////////////////
namespace Tameran
{
	class Stat
	{
	public:
		enum StatType { Regular, Calculated };

	public:
		Stat(std::string _name, StatType _type, std::string _abbreviation, std::string _description);

		//Get
		const std::string Name() const;
		const std::string Abbreviation() const;
		const std::string Description() const;
		const StatType Type() const;

		//Set
		void Name(const std::string _name);
		void Abbreviation(const std::string _abbreviation);
		void Description(const std::string _description);
		void Type(const StatType _type);

	private:
		std::string name;
		StatType type;
		std::string abbreviation;
		std::string description;
	};
}
#endif // STAT

