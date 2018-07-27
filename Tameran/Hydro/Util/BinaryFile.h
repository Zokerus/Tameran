#ifndef LOADBINARYFILE
#define BINARYFILE

//Includes
#include <fstream>
#include <string>

//////////////////////////////////////////////////////////////////////////
//Class: Loading Binary File
//////////////////////////////////////////////////////////////////////////
namespace Hydro
{
	class BinaryFile
	{
	public:
		enum Option{Load, Save};
	public:
		BinaryFile(const std::string path, Option opt)
			: option(opt), file()
		{
			file.open(path);
			if (!file)
			{
				throw std::exception(("Failed open filestream on: " + path).c_str());
			}

			if (!file.good())
			{
				throw std::exception(("Failed open filestream on: " + path).c_str());
			}
		}
		~BinaryFile()
		{}

		template<typename T>
		bool LoadData(T *data)
		{
			size_t test = sizeof(T);
			file.read(reinterpret_cast<char*>(&data), sizeof(T));
			if (!file.good())
			{
				return false;
			}
			return true;
		}

		template<typename T>
		bool LoadData(T &data, int length) 
		{
			file.read(reinterpret_cast<char*>(data), length);
			if (!file.good())
			{
				return false;
			}
			return true;
		}


		bool GetString(std::string *text)
		{
			unsigned short length;
			char* buffer = nullptr;
			file.read(reinterpret_cast<char*>(&length), sizeof(length));
			if (length == 0 || !file.good())
			{
				return false;
			}
			buffer = new char[length];
			file.read(buffer, length);
			if (!file.good())
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
		bool GetString(std::string *text, int length)
		{
			char* buffer = new char[length];
			file.read(buffer, length);
			if (!file.good())
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

		template<typename T>
		void SaveData(T &data)
		{}
	private:
		std::fstream file;
		Option option;
	};
}
#endif // !BINARYFILE

