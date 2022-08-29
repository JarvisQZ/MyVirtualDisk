#pragma once
#include "command.h"
class RenCommand :
	public Command
{
public:
	RenCommand();
	RenCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~RenCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

