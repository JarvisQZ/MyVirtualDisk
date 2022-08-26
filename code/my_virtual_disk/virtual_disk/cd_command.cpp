#include "pch.h"
#include "cd_command.h"


CdCommand::CdCommand()
{
}

CdCommand::CdCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


CdCommand::~CdCommand()
{
}

void CdCommand::Execute(MyVirtualDisk * virtual_disk)
{
	auto command_parameters = this->GetCommandParameters();
	MyDir *current_dir = virtual_disk->GetCurrentDir();
	auto children_dir = current_dir->GetDirChildren();

	if (command_parameters.size() == 0)
	{
		std::cout << virtual_disk->GetCurrentDir()->GetPath() << std::endl;
		std::cout << std::endl;
		return;
	}

	auto path_list = Utils::GetSplitPath(command_parameters[0]);
	
	for (size_t i = 0; i < path_list.size(); ++i)
	{
		if (i == 0 and boost::to_upper_copy(path_list[i]) == "C:")
		{
			current_dir = virtual_disk->GetRootDir();
			children_dir = current_dir->GetDirChildren();
		}
		else if (path_list[i] == ".")
		{
			continue;
		}
		else if (path_list[i] == "..")
		{
			current_dir = current_dir->GetParentDir();
			children_dir = current_dir->GetDirChildren();
		}
		else
		{
			// 查找，如果没有重复文件以及目录，输出错误
			auto all_children = current_dir->GetChildren();
			if (all_children.find(boost::to_upper_copy(path_list[i])) == all_children.end())
			{
				std::cout << "系统找不到指定的路径。" << std::endl;
				std::cout << std::endl;
				return;
			}
			else
			{
				// 找到了，如果是文件，报错
				if (all_children.find(boost::to_upper_copy(path_list[i]))->second->GetType() == FileType::OTHER)
				{
					std::cout << "目录名称无效。" << std::endl;
				}
				else
				{
					current_dir = children_dir[boost::to_upper_copy(path_list[i])];
					children_dir = current_dir->GetDirChildren();
				}
			}
		}
	}
	virtual_disk->SetCurrentDir(*current_dir);
	std::cout << "" << std::endl;
}

bool CdCommand::IsCommandCorrect()
{
	auto command_parameters = this->GetCommandParameters();

	if (command_parameters.size() == 0)
	{
		return true;
	}

	return true;
}
