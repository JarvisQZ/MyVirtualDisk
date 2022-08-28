#include "pch.h"
#include "command_invoker.h"


CommandInvoker::CommandInvoker()
{
}

CommandInvoker::CommandInvoker(Command * command)
	:m_command(command)
{
}


CommandInvoker::~CommandInvoker()
{
	if (m_command != nullptr)
	{
		//delete m_command;
		// 确保m_command被清理
		m_command = nullptr;
	}
}

void CommandInvoker::SetCommand(Command & command)
{
	this->m_command = &command;
}

void CommandInvoker::RunCommand(MyVirtualDisk * virtual_disk)
{
	if (this->m_command->IsCommandCorrect())
	{
		// run
		this->m_command->Execute(virtual_disk);
	}
}
