#pragma once
#include "command.h"
class LoadCommand :
	public Command
{
public:
	LoadCommand();
	LoadCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~LoadCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

