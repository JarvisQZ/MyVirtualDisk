#include "pch.h"
#include "file_type.h"
#include "dir_command.h"


DirCommand::DirCommand()
{
}

DirCommand::DirCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


DirCommand::~DirCommand()
{
}

void DirCommand::Execute(MyVirtualDisk * virtual_disk)
{
	auto command_parameters = this->GetCommandParameters();

	if (command_parameters.empty())
	{
		virtual_disk->GetCurrentDir()->PrintFileAndDir(true);
	}
	else if (command_parameters.size() == 1)
	{
		if (command_parameters[0] == "/s")
		{
			// 递归 打印当前目录所有子目录和文件
			virtual_disk->GetCurrentDir()->PrintFileAndDirRecursion(virtual_disk->GetCurrentDir());
		}
		else if (command_parameters[0] == "/ad")
		{
			// TODO 打印当前目录所有子目录
			virtual_disk->GetCurrentDir()->PrintFileAndDir(false);
		}
		else
		{
			// TODO

			auto children = virtual_disk->GetCurrentDir()->GetChildren();
			auto find = children.find(boost::to_upper_copy(command_parameters[0]));
			// 没有同名文件或者文件夹
			if (find == children.end())
			{
				std::cout << std::endl;
				std::cout << "找不到文件" << std::endl;
				std::cout << std::endl;
			}
			else 
			{
				if (find->second->GetType() == FileType::DIR)
				{
					static_cast<MyDir*>(find->second)->PrintFileAndDir(1);
				}
				else
				{
					//TODO 打印文件
				}
			}
		}
	}
}

bool DirCommand::IsCommandCorrect()
{
	std::deque<std::string> command_parameters = this->GetCommandParameters();

	if (command_parameters.empty())
	{
		return true;
	}
	else if (command_parameters[0][0] == '/')
	{
		if (command_parameters[0] == "/s" || command_parameters[0] == "/ad")
		{
			return true;
		}
		else
		{
			// TODO
			std::cout << "参数格式不正确 - " << "\"" << command_parameters[0] << "\"。" << std::endl;
		}
	}
	else
	{
		return true;
	}

	return false;
}

