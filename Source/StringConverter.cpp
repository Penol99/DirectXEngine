#include "StringConverter.h"

std::wstring StringConverter::StringToWide(std::string aString)
{
	std::wstring wideString(aString.begin(), aString.end());
	return wideString;
}
