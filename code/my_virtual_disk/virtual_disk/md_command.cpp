#include "pch.h"
#include "md_command.h"
#include "file_type.h"


MdCommand::MdCommand()
{
}

MdCommand::MdCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


MdCommand::~MdCommand()
{
}

void MdCommand::Execute(MyVirtualDisk * virtual_disk)
{
	if (this->GetCommandParameters().size() == 1)
	{
		MyDir *current_dir = virtual_disk->GetCurrentDir();
		auto children_dir = current_dir->GetDirChildren();
		auto file_name = this->GetCommandParameters()[0];

		for (auto child : children_dir)
		{
			if (file_name == child.first)
			{
				std::cout << "重名了" << std::endl;
				return;
			}
		}

		MyDir *new_dir = new MyDir(file_name, current_dir->GetPath(), FileType::DIR, current_dir);
		
		current_dir->CreateFileOrDir(file_name, new_dir);

	}
	else if (this->GetCommandParameters().size() > 1)
	{

	}
	std::cout << std::endl;
}

bool MdCommand::IsCommandCorrect()
{
	std::deque<std::string> command_parameters = this->GetCommandParameters();

	if (command_parameters.empty())
	{
		std::cout << "命令语法不正确" << std::endl;
		std::cout << std::endl;
		return false;
	}

	return true;
}
