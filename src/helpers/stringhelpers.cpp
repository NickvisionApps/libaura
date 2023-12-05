#include "helpers/stringhelpers.h"
#include <algorithm>
#include <array>
#include <iomanip>
#include <regex>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#else
#include <uuid/uuid.h>
#endif

namespace Nickvision::Aura
{
	std::string StringHelpers::toLower(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
		return s;
	}

	std::string StringHelpers::toUpper(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
		return s;
	}

	std::string StringHelpers::trim(const std::string& s)
	{
		std::string result{ s };
		result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) 
		{
			return !std::isspace(ch);
		}).base(), result.end());
		result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch)
		{
			return !std::isspace(ch);
		}));
		return result;
	}

	std::string StringHelpers::trim(const std::string& s, char delimiter)
	{
		std::string result{ s };
		result.erase(std::find_if(result.rbegin(), result.rend(), [delimiter](char ch)
			{
				return ch != delimiter;
			}).base(), result.end());
		result.erase(result.begin(), std::find_if(result.begin(), result.end(), [delimiter](char ch)
			{
				return ch != delimiter;
			}));
		return result;
	}

	std::string StringHelpers::newGuid()
	{
		std::array<unsigned char, 16> guid;
#ifdef _WIN32
		GUID win;
		CoCreateGuid(&win);
		guid = {
			(unsigned char)((win.Data1 >> 24) & 0xFF),
			(unsigned char)((win.Data1 >> 16) & 0xFF),
			(unsigned char)((win.Data1 >> 8) & 0xFF),
			(unsigned char)((win.Data1) & 0xff),

			(unsigned char)((win.Data2 >> 8) & 0xFF),
			(unsigned char)((win.Data2) & 0xff),

			(unsigned char)((win.Data3 >> 8) & 0xFF),
			(unsigned char)((win.Data3) & 0xFF),

			(unsigned char)win.Data4[0],
			(unsigned char)win.Data4[1],
			(unsigned char)win.Data4[2],
			(unsigned char)win.Data4[3],
			(unsigned char)win.Data4[4],
			(unsigned char)win.Data4[5],
			(unsigned char)win.Data4[6],
			(unsigned char)win.Data4[7]
		};
#else
		uuid_generate(guid.data());
#endif
		std::ostringstream out;
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[0]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[1]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[2]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[3]);
		out << "-";
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[4]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[5]);
		out << "-";
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[6]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[7]);
		out << "-";
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[8]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[9]);
		out << "-";
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[10]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[11]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[12]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[13]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[14]);
		out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(guid[15]);
		return out.str();
	}

	bool StringHelpers::isValidUrl(const std::string& s)
	{
		if (s.empty())
		{
			return false;
		}
		std::regex urlRegex{ "https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\+~#=]{2,256}\.[a-z]{2,4}\b([-a-zA-Z0-9@:%_\+.~#?&//=]*)" };
		return std::regex_match(s, urlRegex);
	}
}