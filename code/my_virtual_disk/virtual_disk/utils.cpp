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
		<< std::setfill('0') << std::setw(2) << sys_time.wMinute;

	return buf.str();
}

//std::string Utils::GetCommand(std::vector<std::string> command_parameters)
//{
//	return command_parameters[0];
//}

std::deque<std::string> Utils::GetCommandParameters(std::string command)
{
	boost::trim(command);
	std::deque<std::string> command_parameters;
	boost::split(command_parameters, command, boost::is_space());
	return command_parameters;
}

std::vector<std::string> Utils::GetSplitPath(std::string _path)
{
	// 去除路径右边无用的斜杠
	boost::trim_right_if(_path, boost::is_any_of("/\\"));

	std::vector<std::string> path_list;
	boost::split(path_list, _path, boost::is_any_of("/\\"), boost::token_compress_on);
	return path_list;
}