#pragma once
#include "command.h"
#include "command_type.h"
class QuitCommand :
	public Command
{
public:
	QuitCommand();
	QuitCommand(CommandType command_type);
	~QuitCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

