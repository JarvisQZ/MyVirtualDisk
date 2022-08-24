#pragma once
#include "command.h"
#include "command_type.h"
class NoCommand
	:public Command
{
public:
	NoCommand();
	NoCommand(CommandType command_type);
	~NoCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

