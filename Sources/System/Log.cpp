/*

									   \o.o/
---------------------------------.ooO---(_)---Ooo.----------
	Log.cpp
	vkDake01
--------------------------------------------Oooo.-----------
									 .oooO  (   )
									 (   )   ) /
									  \ (   (_/
									   \_)

*/


#include "windows.h"
#include <crtdbg.h>
#include <cstring>
#include <string>
#include <codecvt>

#define	MAXI_DUMP_SIZE		655356
#define	DUMP_COLUMNS		32
char	caDestStr[4096];

// convert UTF-8 string to wstring
std::wstring utf8_to_wstring(const std::string& str) 
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string wstring_to_utf8(const std::wstring& str) 
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.to_bytes(str);
}

void LogInsert(std::string sMessage)
{
	//fprintf(myLogFileID, sMessage.c_str());							// remove logFile, we only log in debugOutput

	std::wstring ourMessage = utf8_to_wstring(sMessage);
	//std::wstring text = L"***Output: " + ourMessage + L"\n";
	OutputDebugString(ourMessage.c_str());
}

void LogInsert(const char* caMessage, ...)
{
	va_list 		list_arg;

	caDestStr[0] = 0;

	va_start(list_arg, caMessage);
	vsprintf_s(caDestStr, caMessage, list_arg);
	va_end(list_arg);

	//fprintf(myLogFileID, "%s", caDestStr);							// remove logFile, we only log in debugOutput

	/*
	std::wstring text = L"***Output: ";
	text += utf8_to_wstring(caDestStr);
	text += L"\n";
	*/
	std::wstring ourMessage = utf8_to_wstring(caDestStr);
	OutputDebugString(ourMessage.c_str());

}

void LogDump(unsigned char* ucBuffer, unsigned int uiLength)
{
	//std::string	sLine ="";
	char		caLine[256];
	char		caByte[8];
	char		caAsciiPart[64];
	char		caAscii[8];

	memset(caLine, 0, 256);
	memset(caAsciiPart, 0, 64);

	if (uiLength <= 0)				return;
	if (uiLength > MAXI_DUMP_SIZE)
	{
		LogInsert("Dump reduced to %d bytes\n", MAXI_DUMP_SIZE);
		uiLength = MAXI_DUMP_SIZE;
	}

	for (unsigned int	uiCounter = 0;
						uiCounter < uiLength;
						uiCounter++)
	{
		sprintf_s(caByte, "%.02X ", ucBuffer[uiCounter]);
		strcat_s(caLine, caByte);

		if (ucBuffer[uiCounter] >= ' ' && ucBuffer[uiCounter] <= 126)
		{
			sprintf_s(caAscii, "%c", ucBuffer[uiCounter]);
			strcat_s(caAsciiPart, caAscii);
		}
		else
		{
			strcat_s(caAsciiPart, ".");
		}

		if (((uiCounter + 1) % DUMP_COLUMNS) == 0)
		{
			LogInsert("0x%.04X - ", uiCounter + 1 - DUMP_COLUMNS);

			LogInsert(caLine);
			LogInsert(" - ");
			LogInsert(caAsciiPart);
			LogInsert("\n");

			memset(caLine, 0, 256);
			memset(caAsciiPart, 0, 64);
		}

	}

}

