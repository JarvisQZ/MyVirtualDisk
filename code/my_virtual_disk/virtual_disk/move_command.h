#pragma once
#include "command.h"
class MoveCommand :
	public Command
{
public:
	MoveCommand();
	MoveCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~MoveCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

