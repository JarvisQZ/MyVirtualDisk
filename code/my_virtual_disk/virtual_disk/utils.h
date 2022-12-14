#pragma once
class MyDir;

class Utils
{
public:
	std::string static GetNowTimeToString();
	//std::string static GetCommand(std::vector<std::string> command_parameters);
	std::deque<std::string> static GetCommandParameters(std::string command);
	std::vector<std::string> static GetSplitPath(std::string _path);
	//std::string static GenerateDirectPath(MyVirtualDisk *virtual_disk);
	bool static IsNameIllegal(std::string name);
	int static IsTargetInDir(MyDir *src_dir, std::string target);
	MyDir static *GetPathDir(std::vector<std::string> path_list, bool is_file);
	bool static WildCardMatching(std::string path, std::string input_path);
};

