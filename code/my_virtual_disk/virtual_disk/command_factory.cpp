#include "pch.h"
#include "command_factory.h"
#include "my_dir.h"
#include "utils.h"
#include "no_command.h"
#include "dir_command.h"
#include "md_command.h"
#include "cd_command.h"
#include "quit_command.h"
#include "cls_command.h"
#include "unknown_command.h"

CommandFactory::CommandFactory()
{
}


CommandFactory::~CommandFactory()
{
}

std::deque<std::string> GetCommandParameters(std::string command)
{
	std::deque<std::string> command_parameters = Utils::GetCommandParameters(command);
	command_parameters.pop_front();
	return command_parameters;
}

std::string GetRawCommand(std::string command)
{
	return Utils::GetCommandParameters(command)[0];
}

CommandType GenerateCommand(std::string command)
{
	CommandType command_type = CommandType::UNKCommand;
	std::string raw_command = GetRawCommand(command);
	std::string command_lower = boost::to_lower_copy(raw_command);

	if (boost::equals("", command_lower))
	{
		command_type = CommandType::NOCommand;
	}
	else if (boost::equals("dir", command_lower))
	{
		command_type = CommandType::DIR;
	}
	else if (boost::equals("cls", command_lower))
	{
		command_type = CommandType::CLS;
	}
	else if (boost::equals("md", command_lower) || boost::equals("mkdir", command_lower))
	{
		command_type = CommandType::MD;
	}
	else if (boost::equals("cd", command_lower))
	{
		command_type = CommandType::CD;
	}
	else if(boost::equals("quit", command_lower) || boost::equals("exit", command_lower))
	{
		command_type = CommandType::QUIT;
	}
	else
	{
		command_type = CommandType::UNKCommand;
	}

	return command_type;
}

Command * CommandFactory::BuildCommand(std::string command, MyDir * dir)
{
	CommandType command_type = GenerateCommand(command);
	std::deque<std::string> command_parameters = GetCommandParameters(command);
	std::string raw_command = GetRawCommand(command);

	switch (command_type)
	{
	case CommandType::DIR:
		return new DirCommand(command_type, command_parameters);
	case CommandType::MD:
		return new MdCommand(command_type, command_parameters);
	case CommandType::RD:
		break;
	case CommandType::CD:
		return new CdCommand(command_type, command_parameters);
	case CommandType::DEL:
		break;
	case CommandType::REN:
		break;
	case CommandType::COPY:
		break;
	case CommandType::MOVE:
		break;
	case CommandType::CLS:
		return new ClsCommand(CommandType::CLS, command_parameters);
	case CommandType::SAVE:
		break;
	case CommandType::LOAD:
		break;
	case CommandType::MKLINK:
		break;
	case CommandType::NOCommand:
		return new NoCommand(CommandType::NOCommand, command_parameters);
	case CommandType::QUIT:
		return new QuitCommand(CommandType::QUIT, command_parameters);
	default:
		return new UnknownCommand(CommandType::UNKCommand, raw_command);
	}
}
