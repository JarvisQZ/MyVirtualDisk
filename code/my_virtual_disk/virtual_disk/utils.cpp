#include "pch.h"
#include "utils.h"

std::string Utils::GetNowTimeToString()
{
	std::ostringstream buf;
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);

	buf << sys_time.wYear << "/" 
		<< std::setfill('0') << std::setw(2) << sys_time.wMonth << "/"
		<< std::setfill('0') << std::setw(2) << sys_time.wDay 
		<< "  " // 两个空格
		<< std::setfill('0') << std::setw(2) << sys_time.wHour << ":" 
		<< std::setfill('0') << std::setw(2) << sys_time.wMinute
		<< std::endl;

	return buf.str();
}

std::string Utils::GetCommand(std::string command)
{
	std::vector<std::string> command_list;
	boost::split(command_list, command, boost::is_space());

	return command_list[0];
}

