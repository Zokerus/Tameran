#include "Stat.h"

Tameran::Stat::Stat(std::string _name, StatType _type, std::string _abbreviation, std::string _description)
	: name(_name), type(_type), abbreviation(_abbreviation), description(_description)
{
}

const std::string Tameran::Stat::Name() const
{
	return name;
}

const std::string Tameran::Stat::Abbreviation() const
{
	return abbreviation;
}

const std::string Tameran::Stat::Description() const
{
	return description;
}

const Tameran::Stat::StatType Tameran::Stat::Type() const
{
	return type;
}

void Tameran::Stat::Name(const std::string _name)
{
	//Checking if the string object is empty
	if (!_name.empty())
	{
		name = _name;
	}
}

void Tameran::Stat::Abbreviation(const std::string _abbreviation)
{
	//Checking if the string object is empty
	if (!_abbreviation.empty())
	{
		abbreviation = _abbreviation;
	}
}

void Tameran::Stat::Description(const std::string _description)
{
	//Checking if the string object is empty
	if (!_description.empty())
	{
		description = _description;
	}
}

void Tameran::Stat::Type(const StatType _type)
{
	//Checking the type object
	if (_type == StatType::Regular || _type == StatType::Calculated)
	{
		type = _type;
	}
}
