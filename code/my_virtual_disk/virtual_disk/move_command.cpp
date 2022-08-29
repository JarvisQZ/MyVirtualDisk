#include "pch.h"
#include "move_command.h"

MoveCommand::MoveCommand()
{
}

MoveCommand::MoveCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


MoveCommand::~MoveCommand()
{
}

void MoveCommand::Execute(MyVirtualDisk * virtual_disk)
{
	std::cout << "MOVE Command!" << std::endl;
}

bool MoveCommand::IsCommandCorrect()
{
	return true;
}
