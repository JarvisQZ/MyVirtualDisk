#pragma once
#include "command.h"
#include "my_virtual_disk.h"
class CommandInvoker
{
public:
	CommandInvoker();
	CommandInvoker(Command* command);
	~CommandInvoker();

	void SetCommand(Command *command);

	void RunCommand(MyVirtualDisk* virtual_disk);

private:
	Command* m_command = nullptr;
};

