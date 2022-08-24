#include "pch.h"
#include "command_factory.h"
#include "my_dir.h"
#include "utils.h"
#include "no_command.h"
#include "quit_command.h"

CommandFactory::CommandFactory()
{
}


CommandFactory::~CommandFactory()
{
}

CommandType GenerateCommand(std::string command)
{
	CommandType command_type;
	std::string res = Utils::GetCommand(command);

	if (res.size() == 0)
	{
		command_type = CommandType::NOCommand;
	}
	else
	{
		command_type = CommandType::QUIT;
	}

	return command_type;
}

Command * CommandFactory::BuildCommand(std::string command, MyDir * dir)
{
	CommandType command_type = GenerateCommand(command);

	switch (command_type)
	{
	case CommandType::DIR:
		break;
	case CommandType::MD:
		break;
	case CommandType::RD:
		break;
	case CommandType::CD:
		break;
	case CommandType::DEL:
		break;
	case CommandType::REN:
		break;
	case CommandType::COPY:
		break;
	case CommandType::MOVE:
		break;
	case CommandType::CLS:
		break;
	case CommandType::SAVE:
		break;
	case CommandType::LOAD:
		break;
	case CommandType::MKLINK:
		break;
	case CommandType::NOCommand:
		return new NoCommand();
	case CommandType::QUIT:
		return new QuitCommand();
	default:
		return new NoCommand();
	}
}