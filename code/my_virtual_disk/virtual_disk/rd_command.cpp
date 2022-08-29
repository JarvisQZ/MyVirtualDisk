#include "pch.h"
#include "rd_command.h"
#include "command_type.h"
#include "my_virtual_disk.h"


RdCommand::RdCommand()
{
}

RdCommand::RdCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


RdCommand::~RdCommand()
{
}

void RdCommand::Execute(MyVirtualDisk * virtual_disk)
{
	std::cout << "RD Command" << std::endl;
}

bool RdCommand::IsCommandCorrect()
{
	return true;
}
