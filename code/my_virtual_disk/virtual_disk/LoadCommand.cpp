#include "pch.h"
#include "LoadCommand.h"


LoadCommand::LoadCommand()
{
}

LoadCommand::LoadCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


LoadCommand::~LoadCommand()
{
}

void LoadCommand::Execute(MyVirtualDisk * virtual_disk)
{
	std::cout << "Load Command!" << std::endl;
}

bool LoadCommand::IsCommandCorrect()
{
	return true;
}
