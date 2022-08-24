#include "pch.h"
#include "quit_command.h"


QuitCommand::QuitCommand()
{
}

QuitCommand::QuitCommand(CommandType command_type)
	:Command(command_type)
{
}


QuitCommand::~QuitCommand()
{
}

void QuitCommand::Execute(MyVirtualDisk * virtual_disk)
{
	virtual_disk->SetQuitFlag(true);
}

bool QuitCommand::IsCommandCorrect()
{
	return true;
}
