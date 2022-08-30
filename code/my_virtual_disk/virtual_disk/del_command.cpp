#include "pch.h"
#include "utils.h"
#include "file_type.h"
#include "del_command.h"


DelCommand::DelCommand()
{
}

DelCommand::DelCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


DelCommand::~DelCommand()
{
}

void DelCommand::Execute(MyVirtualDisk * virtual_disk)
{
	//std::cout << "DEL Command!" << std::endl;

	auto command_parameters = this->GetCommandParameters();
	MyDir *current_dir = virtual_disk->GetCurrentDir();
	auto children_dir = current_dir->GetDirChildren();

	if (command_parameters.size() == 1)
	{
		auto path_list = Utils::GetSplitPath(command_parameters[0]);

		// 删除根目录下的某文件
		if (command_parameters[0][0] == '/')
		{

		}
		else // 删除相对路径下的某文件
		{
			// 遍历到倒数第二个
			for (size_t i = 0; i < path_list.size() - 1; ++i)
			{
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
						std::cout << "找不到 " << current_dir->GenerateDirectPath() << "\\" << path_list[i] << std::endl;
						std::cout << std::endl;
						return;
					}
					else
					{
						// 中间文件名中有文件
						if (child_iter->second->GetType() == FileType::OTHER)
						{
							// TODO
							std::cout << "目录名称无效" << std::endl;
							std::cout << std::endl;
							return;
						}
						else // 找到目录
						{
							current_dir = static_cast<MyDir*>(child_iter->second);
							children_dir = current_dir->GetDirChildren();
						}
					}
				}
			}
			// 最终结果是文件还是目录
			auto all_children = current_dir->GetChildren();
			auto child_iter = all_children.find(boost::to_upper_copy(path_list.back()));
			if (child_iter == all_children.end())
			{
				std::cout << "找不到 " << current_dir->GenerateDirectPath() << "\\" << path_list.back() << std::endl;
				std::cout << std::endl;
				return;
			}
			else
			{
				if (child_iter->second->GetType() == FileType::OTHER)
				{
					// 如果最终结果是文件
					auto del_file = child_iter->second;
					current_dir->DeleteChild(del_file->GetName());
					delete del_file;
					del_file = nullptr;
					std::cout << std::endl;
					return;
				}
				else
				{
					// 是目录 删除该目录下所有文件
					std::string flag = "";
					do
					{
						std::cout << child_iter->second->GenerateDirectPath() << "\\*, " << "是否确认(Y/N)? ";
						std::getline(std::cin, flag);
						boost::to_upper(flag);
					} while (flag[0] != 'N' && flag[0] != 'Y');
					if (flag[0] == 'N')
					{
						// 取消操作
						std::cout << std::endl;
						return;
					}
					else
					{
						// TODO
						// 删除目录下所有文件
						auto del_dir = current_dir->GetDirChildren().find(boost::to_upper_copy(path_list.back()))->second;
						for (auto &child : del_dir->GetChildren())
						{
							if (child.second->GetType() == FileType::OTHER)
							{
								del_dir->DeleteChild(child.second->GetName());
								delete &child;
							}
						}
						std::cout << std::endl;
						return;
					}
				}

			}
		}
	}
}

bool DelCommand::IsCommandCorrect()
{
	auto command_parameters = this->GetCommandParameters();

	if (command_parameters.size() == 0)
	{
		std::cout << "命令语法不正确。" << std::endl;
		std::cout << std::endl;
		return false;
	}
	else if (command_parameters.size() == 1 &&
		command_parameters[0].size() > 0 &&
		command_parameters[0][0] == '/')
	{
		if (boost::to_lower_copy(command_parameters[0]) != "/s")
		{
			std::cout << "参数格式不正确 - " << "\"" << command_parameters[0] << "\"。" << std::endl;
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
