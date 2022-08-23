#include "pch.h"
#include "utils.h"

std::string Utils::GetNowTimeToString()
{
	time_t now = time(0);

	tm *ltm = localtime(&now);

	char tempBuf[255];

	std::strftime(tempBuf,255, "%Y-%m-%d  %H:%M:%S", ltm);
	return tempBuf;
}
