#include "pch.h"
#include "del_command.h"


DelCommand::DelCommand()
{
}

DelCommand::DelCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


DelCommand::~DelCommand()
{
}

void DelCommand::Execute(MyVirtualDisk * virtual_disk)
{
	std::cout << "DEL Command!" << std::endl;
}

bool DelCommand::IsCommandCorrect()
{
	return true;
}
