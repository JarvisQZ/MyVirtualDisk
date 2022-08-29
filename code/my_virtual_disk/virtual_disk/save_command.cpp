#include "pch.h"
#include "save_command.h"


SaveCommand::SaveCommand()
{
}

SaveCommand::SaveCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


SaveCommand::~SaveCommand()
{
}

void SaveCommand::Execute(MyVirtualDisk * virtual_disk)
{
	std::cout << "Save Command!" << std::endl;
}

bool SaveCommand::IsCommandCorrect()
{
	return true;
}
