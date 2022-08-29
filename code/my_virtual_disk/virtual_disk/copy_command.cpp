#include "pch.h"
#include "copy_command.h"


CopyCommand::CopyCommand()
{
}

CopyCommand::CopyCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


CopyCommand::~CopyCommand()
{
}

void CopyCommand::Execute(MyVirtualDisk * virtual_disk)
{
	std::cout << "COPY Command!" << std::endl;
}

bool CopyCommand::IsCommandCorrect()
{
	return true;
}
