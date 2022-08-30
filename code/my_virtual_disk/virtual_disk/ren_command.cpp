#include "pch.h"
#include "utils.h"
#include "file_type.h"
#include "ren_command.h"


RenCommand::RenCommand()
{
}

RenCommand::RenCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


RenCommand::~RenCommand()
{
}

void RenCommand::Execute(MyVirtualDisk * virtual_disk)
{
	//std::cout << "REN Command" << std::endl;

	auto command_parameters = this->GetCommandParameters();
	auto src_ = command_parameters[0];
	auto dst_ = command_parameters[1];
	MyDir *current_dir = virtual_disk->GetCurrentDir();
	auto children_dir = current_dir->GetDirChildren();

	auto path_list = Utils::GetSplitPath(src_);
	auto dst_path_list = Utils::GetSplitPath(dst_);

	if (dst_path_list.size() != 1 or Utils::IsNameIllegal(dst_))
	{
		std::cout << "命令语法不正确。" << std::endl;
		std::cout << std::endl;
		return;
	}

	// 遍历路径，定位到要改名的文件夹或者文件的上一级
	for (size_t i = 0; i < path_list.size() - 1; ++i)
	{
		// 绝对路径
		if (i == 0 and (path_list[i] == "C:" or path_list[0][0] == '/'))
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
		// 相对路径
		else if (path_list[i] == ".")
		{
			//if (i == path_list.size() - 1)
			//{
			//	std::cout << "err" << std::endl;
			//	return;
			//}
			continue;
		}
		else if (path_list[i] == "..")
		{
			//if (i == path_list.size() - 1)
			//{
			//	std::cout << "err" << std::endl;
			//	return;
			//}
			current_dir = current_dir->GetParentDir();
			children_dir = current_dir->GetDirChildren();
		}
		else
		{
			// 查找，如果没有对应目录，返回错误信息
			auto all_children = current_dir->GetChildren();
			auto child_iter = all_children.find(boost::to_upper_copy(path_list[i]));
			if (child_iter == all_children.end())
			{
				std::cout << "系统找不到指定的文件。" << std::endl;
				std::cout << std::endl;
				return;
			}
			else
			{
				// 找到子目录或文件
				if (child_iter->second->GetType() == FileType::OTHER)
				{
					std::cout << "系统找不到指定的路径。" << std::endl;
					std::cout << std::endl;
					return;
				}
				else
				{
					current_dir = static_cast<MyDir*>(child_iter->second);
					children_dir = current_dir->GetDirChildren();
				}
			}
		}
	}

	// 找到最终文件或目录
	auto all_children = current_dir->GetChildren();
	auto child_iter = all_children.find(boost::to_upper_copy(path_list.back()));
	auto dst_iter = all_children.find(boost::to_upper_copy(dst_));
	if (child_iter == all_children.end())
	{
		std::cout << "系统找不到指定的文件。" << std::endl;
		std::cout << std::endl;
		return;
	}
	else if (dst_iter != all_children.end()) // 目标文件重名
	{
		std::cout << "存在一个重名文件，或是找不到文件。" << std::endl;
		std::cout << std::endl;
		return;
	}
	else // 可以进行重命名
	{
		auto src_file = child_iter->second;
		// 如果 src 是个文件
		if (src_file->GetType() == FileType::OTHER)
		{
			// 先改名字，再向map中添加一个相同的指针，去除之前的指针
			src_file->SetName(dst_);
			src_file->SetPath(current_dir->GenerateDirectPath() + "\\" + dst_);
			current_dir->CreateFileOrDir(dst_, child_iter->second);
			//current_dir->DeleteChild(src_);
			current_dir->DeleteChild(path_list.back());

			std::cout << std::endl;
			return;
		}
		else // src 是个文件夹
		{
			auto src_children = static_cast<MyDir *>(src_file)->GetChildren();
			for (auto &child : src_children)
			{
				// TODO 
				// 未实现 应递归, 弃用
				// 或者将filepath 改成栈来实现， 弃用
				// 实现新的方法生成绝对路径
				child.second->SetPath(current_dir->GenerateDirectPath() + "\\" + dst_ + "\\" + child.second->GetName());
			}
			src_file->SetPath(current_dir->GenerateDirectPath() + "\\" + dst_);
			src_file->SetName(dst_);
			current_dir->CreateFileOrDir(dst_, child_iter->second);
			//current_dir->DeleteChild(src_);
			current_dir->DeleteChild(path_list.back());

			std::cout << std::endl;
			return;
		}

	}

}

bool RenCommand::IsCommandCorrect()
{
	auto command_parameters = this->GetCommandParameters();

	if (command_parameters.size() != 2)
	{
		std::cout << "命令语法不正确。" << std::endl;
		std::cout << std::endl;
		return false;
	}
	return true;
}
