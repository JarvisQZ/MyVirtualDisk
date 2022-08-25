#pragma once
#include "command.h"
#include "command_type.h"
class UnknownCommand :
	public Command
{
public:
	UnknownCommand();
	UnknownCommand(CommandType command_type, std::string raw_command);
	~UnknownCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();

private:
	std::string m_raw_command;
};

