#pragma once
#include "command.h"
#include "command_type.h"
#include "my_dir.h"
class DirCommand :
	public Command
{
public:
	DirCommand();
	DirCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~DirCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

