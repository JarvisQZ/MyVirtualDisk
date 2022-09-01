#include "pch.h"
#include "utils.h"
#include "rd_command.h"
#include "command_type.h"
#include "my_virtual_disk.h"
#include "my_file.h"
#include "file_type.h"


RdCommand::RdCommand()
{
}

RdCommand::RdCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}

RdCommand::~RdCommand()
{
}

void run_delete_dir(MyDir *current_dir)
{
	auto virtual_disk = MyVirtualDisk::GetInstance();

	// 要删除的目录非空，返回错误信息
	if (current_dir->GetChildren().size() != 0)
	{
		std::cout << "目录不是空的。" << std::endl;
		std::cout << std::endl;
		return;
	}
	if (current_dir == virtual_disk->GetCurrentDir())
	{
		std::cout << "另一个程序正在使用此文件，进程无法访问。" << std::endl;
		std::cout << std::endl;
		return;
	}
	auto parent_dir = current_dir->GetParentDir();
	parent_dir->DeleteChild(current_dir->GetName());
	delete current_dir;
}

void run_delete_dir_recursion(MyDir *current_dir)
{
	auto virtual_disk = MyVirtualDisk::GetInstance();

	for (auto &_dir : current_dir->GetDirChildren())
	{
		run_delete_dir_recursion(_dir.second);
	}
	if (current_dir == virtual_disk->GetCurrentDir())
	{
		std::cout << "另一个程序正在使用此文件，进程无法访问。" << std::endl;
		std::cout << std::endl;
		return;
	}
	auto parent_dir = current_dir->GetParentDir();
	parent_dir->DeleteChild(current_dir->GetName());
	delete current_dir;
}

void RdCommand::Execute(MyVirtualDisk * virtual_disk)
{
	//std::cout << "RD Command" << std::endl;

	auto command_parameters = this->GetCommandParameters();
	//MyDir *current_dir = virtual_disk->GetCurrentDir();
	//auto children_dir = current_dir->GetDirChildren();

	if (command_parameters.size() == 1)
	{
		auto path_list = Utils::GetSplitPath(command_parameters[0]);

		// 
		auto target_dir = Utils::GetPathDir(path_list, false);
		if (target_dir == nullptr)
		{
			std::cout << std::endl;
			return;
		}

		run_delete_dir(target_dir);
		std::cout << std::endl;
		return;

		//// 删除根目录下的某目录
		//if (command_parameters[0][0] == '/')
		//{

		//}
		//else // 删除相对路径下的目录
		//{
		//	for (size_t i = 0; i < path_list.size(); ++i)
		//	{
		//		if (i == 0 and path_list[i] == "C:")
		//		{
		//			if (i == path_list.size() - 1)
		//			{
		//				std::cout << "err" << std::endl;
		//				return;
		//			}
		//			//TODO 根目录情况
		//			current_dir = virtual_disk->GetRootDir();
		//			children_dir = current_dir->GetDirChildren();
		//		}
		//		else if (path_list[i] == ".")
		//		{
		//			continue;
		//		}
		//		else if (path_list[i] == "..")
		//		{
		//			current_dir = current_dir->GetParentDir();
		//			children_dir = current_dir->GetDirChildren();
		//		}
		//		else
		//		{
		//			// 查找，如果没有对应目录，返回错误信息
		//			auto all_children = current_dir->GetChildren();
		//			auto child_iter = all_children.find(boost::to_upper_copy(path_list[i]));
		//			if (child_iter == all_children.end())
		//			{
		//				std::cout << "系统找不到指定的路径。" << std::endl;
		//				std::cout << std::endl;
		//				return;
		//			}
		//			else
		//			{
		//				// 找到子目录或文件
		//				if (child_iter->second->GetType() == FileType::OTHER)
		//				{
		//					std::cout << "目录名称无效。" << std::endl;
		//					std::cout << std::endl;
		//					return;
		//				}
		//				else
		//				{
		//					current_dir = static_cast<MyDir*>(child_iter->second);
		//					children_dir = current_dir->GetDirChildren();
		//				}
		//			}
		//		}
		//	}
		//	// 要删除的目录非空，返回错误信息
		//	if (children_dir.size() != 0)
		//	{
		//		std::cout << "目录不是空的。" << std::endl;
		//		std::cout << std::endl;
		//		return;
		//	}
		//	auto del_dir = current_dir;
		//	if (del_dir == virtual_disk->GetCurrentDir())
		//	{
		//		std::cout << "另一个程序正在使用此文件，进程无法访问。" << std::endl;
		//		std::cout << std::endl;
		//		return;
		//	}
		//	current_dir = current_dir->GetParentDir();
		//	current_dir->DeleteChild(del_dir->GetName());
		//	delete del_dir;
		//	std::cout << std::endl;
		//	return;
		//}
	}
	else // 多个参数
	{
		if (command_parameters[0] == "/s")
		{
			// rd 命令的参数得是文件夹

			auto path_list = Utils::GetSplitPath(command_parameters[1]);

			auto target_dir = Utils::GetPathDir(path_list, false);

			// 要删除目录下所有文件以及目录。先遍历删除所有的文件
			target_dir->DeleteFileInDirRecursion(target_dir);

			if (target_dir == nullptr)
			{
				std::cout << std::endl;
				return;
			}

			run_delete_dir_recursion(target_dir);
			std::cout << std::endl;
			return;
		}
	}
}

bool RdCommand::IsCommandCorrect()
{
	auto command_parameters = this->GetCommandParameters();

	if (command_parameters.size() == 0)
	{
		std::cout << "命令语法不正确。" << std::endl;
		std::cout << std::endl;
		return false;
	}
	else if (command_parameters.size() == 1
		&& command_parameters[0].size() > 0
		&& command_parameters[0][0] == '/')
	{
		if (boost::to_lower_copy(command_parameters[0]) != "/s")
		{
			std::cout << "无效开关 - " << "\"" << command_parameters[0] << "\"。" << std::endl;
			std::cout << std::endl;
			return false;
		}
		else
		{
			std::cout << "命令语法不正确。" << std::endl;
			std::cout << std::endl;
			return false;
		}
	}

	return true;
}