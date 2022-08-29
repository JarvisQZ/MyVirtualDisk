#include "pch.h"
#include "cls_command.h"


ClsCommand::ClsCommand()
{
}

ClsCommand::ClsCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


ClsCommand::~ClsCommand()
{
}

void ClsCommand::Execute(MyVirtualDisk * virtual_disk)
{
	system("cls");
	std::cout << std::endl;
}

bool ClsCommand::IsCommandCorrect()
{
	return true;
}
