#ifndef STATMANAGER
#define STATMANAGER

#include <fstream>
#include <map>
#include <string>

//My Includes
#include "Stat.h"

//////////////////////////////////////////////////////////////////////////
//Class: StatManager
//////////////////////////////////////////////////////////////////////////
namespace Tameran
{
	class StatManager
	{
	public:
		StatManager();

		bool LoadStats(const std::string& path);
		bool SaveStats(const std::string& path); //Probably unnecessary

	private:
		bool GetString(std::ifstream& in, std::string* text);

	private:
		std::map<std::string, Stat> stats;
	};
}
#endif // !STATMANAGER

