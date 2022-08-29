#pragma once
#include "command.h"
class CopyCommand :
	public Command
{
public:
	CopyCommand();
	CopyCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~CopyCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

