#include "pch.h"
#include "no_command.h"


NoCommand::NoCommand()
{
}

NoCommand::NoCommand(CommandType command_type)
	:Command(command_type)
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