#pragma once
#include "command.h"
#include "command_type.h"
#include "my_dir.h"

class CommandFactory
{
public:
	CommandFactory();
	~CommandFactory();

	Command* BuildCommand(std::string command, MyDir* dir);
};

