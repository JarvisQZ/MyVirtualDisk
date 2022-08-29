#pragma once
#include "command.h"
class MklinkCommand :
	public Command
{
public:
	MklinkCommand();
	MklinkCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~MklinkCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

