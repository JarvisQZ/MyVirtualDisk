#pragma once
#include "command.h"
class RdCommand
	:public Command
{
public:
	RdCommand();
	RdCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~RdCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

