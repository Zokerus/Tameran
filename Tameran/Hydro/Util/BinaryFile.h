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
			: file(), option(opt), length(0)
		{
			if (option == Option::Load)
			{
				file.open(path, std::ios::in | std::ios::binary);
			}
			else
			{
				file.open(path, std::ios::out | std::ios::binary);
			}
			
			if (!file)
			{
				throw std::exception(("Failed open filestream on: " + path).c_str());
			}

			if (!file.good())
			{
				throw std::exception(("Failed open filestream on: " + path).c_str());
			}

			// get length of file:
			file.seekg(0, file.end);
			length = (unsigned int)file.tellg();
			file.seekg(0, file.beg);
		}
		~BinaryFile()
		{}

		template<typename T>
		void LoadData(T *data)
		{
			size_t test = sizeof(T);
			file.read(reinterpret_cast<char*>(data), sizeof(T));
			if (!file.good())
			{
				throw std::exception();
			}
		}

		template<typename T>
		void LoadData(T *data, int length) 
		{
			file.read(reinterpret_cast<char*>(data), length);
			if (!file.good())
			{
				throw std::exception();
			}
		}


		void GetString(std::string *text)
		{
			unsigned short length;
			char* buffer = nullptr;
			file.read(reinterpret_cast<char*>(&length), sizeof(length));
			if (length == 0 || !file.good())
			{
				throw std::exception();
			}
			buffer = new char[length];
			file.read(buffer, length);
			if (!file.good())
			{
				delete[] buffer;
				buffer = nullptr;
				throw std::exception();
			}
			*text = "";
			text->append(buffer, length);
			if (length != text->length())
			{
				delete[] buffer;
				buffer = nullptr;
				throw std::exception();
			}
			delete[] buffer;
			buffer = nullptr;
		}
		void GetString(std::string *text, int length)
		{
			char* buffer = new char[length];
			file.read(buffer, length);
			if (!file.good())
			{
				delete[] buffer;
				buffer = nullptr;
				throw std::exception();
			}
			*text = "";
			text->append(buffer, length);
			if (length != text->length())
			{
				delete[] buffer;
				buffer = nullptr;
				throw std::exception();
			}
			delete[] buffer;
			buffer = nullptr;
		}

		template<typename T>
		void SaveData(T &data)
		{}

		bool EndOfFile()
		{
			return file.eof();
		}

		bool Error()
		{
			return file.eof() | file.fail() | file.bad();
		}

		unsigned int FileLength()
		{
			return length;
		}

		int TillEnd()
		{
			return (int)(length - (unsigned int)file.tellg());
		}
	private:
		std::fstream file;
		Option option;
		unsigned int length;
	};
}
#endif // !BINARYFILE

