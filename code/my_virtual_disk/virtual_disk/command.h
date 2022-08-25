#pragma once
#include "my_virtual_disk.h"
#include "command_type.h"
class Command
{
public:
	Command();
	Command(CommandType command_type);
	Command(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~Command();

	virtual void Execute(MyVirtualDisk* virtual_disk) = 0;
	virtual bool IsCommandCorrect() = 0;

	virtual CommandType GetCommandType();
	virtual std::deque<std::string> GetCommandParameters();

private:
	CommandType m_command_type = CommandType::NOCommand;
	std::deque<std::string> m_command_parameters;
};

