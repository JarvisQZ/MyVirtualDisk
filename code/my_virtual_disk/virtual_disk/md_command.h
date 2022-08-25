#pragma once
#include "command.h"
#include "my_dir.h"
class MdCommand :
	public Command
{
public:
	MdCommand();
	MdCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~MdCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};
