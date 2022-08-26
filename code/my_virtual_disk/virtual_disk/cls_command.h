#pragma once
#include"command.h"
#include"command_type.h"
class ClsCommand
	: public Command
{
public:
	ClsCommand();
	ClsCommand(CommandType command_type, std::deque<std::string> command_parameters);
	~ClsCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

