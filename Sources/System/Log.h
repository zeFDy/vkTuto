/*

									   \o.o/
---------------------------------.ooO---(_)---Ooo.----------
	Log.h
	vkDake01
--------------------------------------------Oooo.-----------
									 .oooO  (   )
									 (   )   ) /
									  \ (   (_/
									   \_)

*/

#pragma once
#include <string>

void LogInsert(std::string sMessage);
void LogInsert(const char* caMessage, ...);
void LogDump(unsigned char* ucBuffer, unsigned int iLength);
