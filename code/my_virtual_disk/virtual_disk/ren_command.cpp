#include "pch.h"
#include "ren_command.h"


RenCommand::RenCommand()
{
}

RenCommand::RenCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


RenCommand::~RenCommand()
{
}

void RenCommand::Execute(MyVirtualDisk * virtual_disk)
{
	std::cout << "REN Command" << std::endl;
}

bool RenCommand::IsCommandCorrect()
{
	return true;
}
