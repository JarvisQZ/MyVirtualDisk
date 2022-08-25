#pragma once
class Utils
{
public:
	std::string static GetNowTimeToString();
	//std::string static GetCommand(std::vector<std::string> command_parameters);
	std::deque<std::string> static GetCommandParameters(std::string command);

	std::vector<std::string> GetSplitPath(std::deque<std::string> command);
};

