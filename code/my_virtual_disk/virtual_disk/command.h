#pragma once
#include "my_virtual_disk.h"
#include "command_type.h"
class Command
{
public:
	Command();
	Command(CommandType command_type);
	virtual ~Command();

	virtual void Execute(MyVirtualDisk* virtual_disk) = 0;
	virtual bool IsCommandCorrect() = 0;

	virtual CommandType GetCommandType();

private:
	CommandType m_command_type = CommandType::NOCommand;
};

