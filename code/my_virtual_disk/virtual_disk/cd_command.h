#pragma once
#include "command.h"
#include "command_type.h"
class CdCommand :
	public Command
{
public:
	CdCommand();
	CdCommand(CommandType command_type, std::deque<std::string> command_parameters);
	~CdCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

