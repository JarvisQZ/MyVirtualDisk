#pragma once
#include "command.h"
#include "command_type.h"
class QuitCommand :
	public Command
{
public:
	QuitCommand();
	QuitCommand(CommandType command_type, std::deque<std::string> command_parameters);
	~QuitCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

