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

Command::Command(CommandType command_type, std::deque<std::string> command_parameters)
	: m_command_type(command_type), m_command_parameters(command_parameters)
{
}


Command::~Command()
{
}

CommandType Command::GetCommandType()
{
	return this->m_command_type;
}

std::deque<std::string> Command::GetCommandParameters()
{
	return this->m_command_parameters;
}
