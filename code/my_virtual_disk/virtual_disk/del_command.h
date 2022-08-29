#pragma once
#include "command.h"
class DelCommand :
	public Command
{
public:
	DelCommand();
	DelCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~DelCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

