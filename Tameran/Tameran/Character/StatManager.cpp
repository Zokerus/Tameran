#include "StatManager.h"

Tameran::StatManager::StatManager()
	: stats()
{
	LoadStats("Game/Character/Stats/Stats.dat");
}

bool Tameran::StatManager::LoadStats(const std::string& path)
{
	//TODO Load Stat file

	unsigned short countStats;
	std::string name, abb, desc;
	Stat::StatType type = Stat::Regular;

	std::ifstream in(path, std::ios::binary);
	if (!in)
	{
		return false;
	}

	in.read(reinterpret_cast<char*>(&countStats), sizeof(countStats));
	if (countStats == 0 || !in.good())
	{
		return false;
	}

	for (int i = 0; in.good() && i < countStats; i++)
	{
		if (!GetString(in, &name))
		{
			return false;
		}
		if (!GetString(in, &abb))
		{
			return false;
		}
		if (!GetString(in, &desc))
		{
			return false;
		}
		in.read(reinterpret_cast<char*>(&type), sizeof(unsigned short));
		if(!in.good())
		{
			return false;
		}

		stats.emplace(name, Stat(name, type, abb, desc));
	}

	return true;
}

bool Tameran::StatManager::SaveStats(const std::string& path)
{
	//TODO Save Stats
	return false;
}

bool Tameran::StatManager::GetString(std::ifstream& in, std::string* text)
{
	unsigned short length;
	char* buffer = nullptr;
	in.read(reinterpret_cast<char*>(&length), sizeof(length));
	if (length == 0 || !in.good())
	{
		return false;
	}
	buffer = new char[length];
	in.read(buffer, length);
	if (!in.good())
	{
		delete[] buffer;
		buffer = nullptr;
		return false;
	}
	*text = "";
	text->append(buffer, length);
	if (length != text->length())
	{
		delete[] buffer;
		buffer = nullptr;
		return false;
	}
	delete[] buffer;
	buffer = nullptr;
	return true;
}
