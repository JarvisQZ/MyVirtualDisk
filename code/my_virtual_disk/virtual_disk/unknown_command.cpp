#include "pch.h"
#include "unknown_command.h"


UnknownCommand::UnknownCommand()
{
}

UnknownCommand::UnknownCommand(CommandType command_type, std::string raw_command)
	:Command(command_type), m_raw_command(raw_command)
{
}


UnknownCommand::~UnknownCommand()
{
}

void UnknownCommand::Execute(MyVirtualDisk * virtual_disk)
{
	std::cout << "'" << this->m_raw_command << "'" << " 不是内部或外部命令，也不是可运行的程序" << std::endl;
	std::cout << "或批处理文件。" << std::endl;
}

bool UnknownCommand::IsCommandCorrect()
{
	return true;
}
