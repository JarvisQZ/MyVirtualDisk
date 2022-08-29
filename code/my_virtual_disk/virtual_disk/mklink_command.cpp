#include "pch.h"
#include "mklink_command.h"


MklinkCommand::MklinkCommand()
{
}

MklinkCommand::MklinkCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


MklinkCommand::~MklinkCommand()
{
}

void MklinkCommand::Execute(MyVirtualDisk * virtual_disk)
{
	std::cout << "MKLINK Command!" << std::endl;
}

bool MklinkCommand::IsCommandCorrect()
{
	return true;
}
