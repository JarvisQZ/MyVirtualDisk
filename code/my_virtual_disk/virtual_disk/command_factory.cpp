#include "pch.h"
#include "command_factory.h"
#include "my_dir.h"
#include "utils.h"
#include "no_command.h"
#include "dir_command.h"
#include "md_command.h"
#include "rd_command.h"
#include "cd_command.h"
#include "del_command.h"
#include "copy_command.h"
#include "ren_command.h"
#include "move_command.h"
#include "mklink_command.h"
#include "save_command.h"
#include "LoadCommand.h"
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
	else if (boost::equals("rd", command_lower))
	{
		command_type = CommandType::RD;
	}
	else if (boost::equals("del", command_lower))
	{
		command_type = CommandType::DEL;
	}
	else if (boost::equals("copy", command_lower))
	{
		command_type = CommandType::COPY;
	}
	else if (boost::equals("ren", command_lower))
	{
		command_type = CommandType::REN;
	}
	else if (boost::equals("move", command_lower))
	{
		command_type = CommandType::MOVE;
	}
	else if (boost::equals("mklink", command_lower))
	{
		command_type = CommandType::MKLINK;
	}
	else if (boost::equals("save", command_lower))
	{
		command_type = CommandType::SAVE;
	}
	else if (boost::equals("load", command_lower))
	{
		command_type = CommandType::LOAD;
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
		return new RdCommand(command_type, command_parameters);
	case CommandType::CD:
		return new CdCommand(command_type, command_parameters);
	case CommandType::DEL:
		return new DelCommand(command_type, command_parameters);
	case CommandType::REN:
		return new RenCommand(command_type, command_parameters);
	case CommandType::COPY:
		return new CopyCommand(command_type, command_parameters);
	case CommandType::MOVE:
		return new MoveCommand(command_type, command_parameters);
	case CommandType::CLS:
		return new ClsCommand(command_type, command_parameters);
	case CommandType::SAVE:
		return new SaveCommand(command_type, command_parameters);
	case CommandType::LOAD:
		return new LoadCommand(command_type, command_parameters);
	case CommandType::MKLINK:
		return new MklinkCommand(command_type, command_parameters);
	case CommandType::NOCommand:
		return new NoCommand(command_type, command_parameters);
	case CommandType::QUIT:
		return new QuitCommand(command_type, command_parameters);
	default:
		return new UnknownCommand(command_type, raw_command);
	}
}
