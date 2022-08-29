#pragma once
#include "command.h"
class SaveCommand :
	public Command
{
public:
	SaveCommand();
	SaveCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~SaveCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

