#pragma once
#include "command.h"
class RdCommand
	:public Command
{
public:
	RdCommand();
	RdCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~RdCommand();

	void delete_dir(MyDir &current_dir, std::map<std::string, MyDir*> &children_dir, std::vector<std::string> &path_list);
	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

