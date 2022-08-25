#include "pch.h"
#include "dir_command.h"


DirCommand::DirCommand()
{
}

DirCommand::DirCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


DirCommand::~DirCommand()
{
}

void DirCommand::Execute(MyVirtualDisk * virtual_disk)
{
	if (this->GetCommandParameters().empty())
	{
		virtual_disk->GetCurrentDir()->PrintFileAndDir();
	}
}

bool DirCommand::IsCommandCorrect()
{
	std::deque<std::string> command_parameters = this->GetCommandParameters();
	
	if (command_parameters.empty())
	{
		return true;
	}

	return false;
}

