#include "pch.h"
#include "command.h"
#include "command_type.h"


Command::Command()
{
}

Command::Command(CommandType command_type)
	:m_command_type(command_type)
{
}


Command::~Command()
{
}

CommandType Command::GetCommandType()
{
	return this->m_command_type;
}
