#include "pch.h"
#include "no_command.h"


NoCommand::NoCommand()
{
}

NoCommand::NoCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


NoCommand::~NoCommand()
{
}

void NoCommand::Execute(MyVirtualDisk * virtual_disk)
{// enpty command do nothing
}

bool NoCommand::IsCommandCorrect()
{
	return true;
}