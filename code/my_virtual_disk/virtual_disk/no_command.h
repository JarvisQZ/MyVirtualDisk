#pragma once
#include "command.h"
#include "command_type.h"
class NoCommand
	:public Command
{
public:
	NoCommand();
	NoCommand(CommandType command_type, std::deque<std::string> command_parameters);
	~NoCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

