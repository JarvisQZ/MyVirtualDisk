#include "pch.h"
#include "utils.h"
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
	auto command_parameters = this->GetCommandParameters();
	MyDir *current_dir = virtual_disk->GetCurrentDir();
	auto children_dir = current_dir->GetDirChildren();

	if (command_parameters.size() == 1)
	{
		auto path_list = Utils::GetSplitPath(command_parameters[0]);

		for (size_t i = 0; i < path_list.size(); ++i)
		{

			if (Utils::IsNameIllegal(path_list[i]))
			{
				std::cout << "文件名、目录名或卷标语法不正确。" << std::endl;
				std::cout << std::endl;
				return;
			}

			if (i == 0 and path_list[i] == "C:")
			{
				if (i == path_list.size() - 1)
				{
					std::cout << "err" << std::endl;
					return;
				}
				//TODO 根目录情况
				current_dir = virtual_disk->GetRootDir();
				children_dir = current_dir->GetDirChildren();
			}
			else if (path_list[i] == ".")
			{
				if (i == path_list.size() - 1)
				{
					std::cout << "err" << std::endl;
					return;
				}
				continue;
			}
			else if (path_list[i] == "..")
			{
				if (i == path_list.size() - 1)
				{
					std::cout << "err" << std::endl;
					return;
				}
				current_dir = current_dir->GetParentDir();
				children_dir = current_dir->GetDirChildren();
			}
			else
			{
				// 查找，如果没有重复文件以及目录，创建新目录
				auto all_children = current_dir->GetChildren();
				if (all_children.find(boost::to_upper_copy(path_list[i])) == all_children.end())
				{
					MyDir *new_dir;

					// 如果在根目录创建的文件夹
					if (current_dir == virtual_disk->GetRootDir())
					{
						new_dir = new MyDir(path_list[i], current_dir->GenerateDirectPath() + path_list[i], FileType::DIR, current_dir);
					}
					else
					{
						// 需要在父目录下创建的文件夹
						new_dir = new MyDir(path_list[i], current_dir->GenerateDirectPath() + "\\" + path_list[i], FileType::DIR, current_dir);
					}

					// 创建文件夹
					current_dir->AddChild(path_list[i], new_dir);
					current_dir = new_dir;
					children_dir = current_dir->GetDirChildren();
				}
				else
				{
					// 存在重名目录或者文件

					std::cout << "子目录或文件 " << path_list[i] << " 已经存在。" << std::endl;
					std::cout << std::endl;
				}
			}
		}
	}
	else if (command_parameters.size() > 1)
	{
		for (auto _path : command_parameters)
		{
			auto path_list = Utils::GetSplitPath(_path);
			//TODO
		}
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
	else if (command_parameters[0][0] == '/' && command_parameters[0][1] == '?')
	{
		//可以写成help类
		std::cout << "help" << std::endl;

		return false;
	}

	return true;
}
