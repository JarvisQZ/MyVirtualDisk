#include "pch.h"
#include "utils.h"
#include "my_file.h"
#include "my_link_dir.h"
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

// 删除一个文件
void run_delete(MyFile * del_file)
{
	auto virtual_disk = MyVirtualDisk::GetInstance();
	auto current_dir = virtual_disk->GetCurrentDir();
	current_dir->DeleteChild(del_file->GetName());
	delete del_file;
	del_file = nullptr;
}

void run_delete(MyDir* targetdir)
{
	// 删除当前目录下的文件
	std::string flag = "";
	do
	{
		std::cout << targetdir->GenerateDirectPath() << "\\*, " << "是否确认(Y/N)? ";
		std::getline(std::cin, flag);
		boost::to_upper(flag);
	} while (flag[0] != 'N' && flag[0] != 'Y');
	if (flag[0] == 'N')
	{
		// 取消操作
		std::cout << std::endl;
		return;
	}
	else {
		targetdir->DeleteFileInDir(targetdir);
		std::cout << std::endl;
	}
}

void run_delete_recursion(MyDir* targetdir)
{
	// 删除当前目录下的文件
	std::string flag = "";
	do
	{
		std::cout << targetdir->GenerateDirectPath() << "\\*, " << "是否确认(Y/N)? ";
		std::getline(std::cin, flag);
		boost::to_upper(flag);
	} while (flag[0] != 'N' && flag[0] != 'Y');
	if (flag[0] == 'N')
	{
		// 取消操作
		std::cout << std::endl;
		return;
	}
	else {
		targetdir->DeleteFileInDirRecursion(targetdir);
		std::cout << std::endl;
	}
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

		// 不确定是不是文件，按照文件处理
		current_dir = Utils::GetPathDir(path_list, true);
		if (current_dir == nullptr)
		{
			std::cout << std::endl;
			return;
		}

		if (path_list.back() == ".")
		{
			// 删除当前目录下的文件
			run_delete(current_dir);

			//std::string flag = "";
			//do
			//{
			//	std::cout << current_dir->GenerateDirectPath() << "\\*, " << "是否确认(Y/N)? ";
			//	std::getline(std::cin, flag);
			//	boost::to_upper(flag);
			//} while (flag[0] != 'N' && flag[0] != 'Y');
			//if (flag[0] == 'N')
			//{
			//	// 取消操作
			//	std::cout << std::endl;
			//	return;
			//}
			//else {
			//	this->MyDelete(current_dir);
			//	std::cout << std::endl;
			//}
		}
		else if (path_list.back() == "..")
		{
			// 删除上一级目录下的文件
			current_dir = current_dir->GetParentDir();
			run_delete(current_dir);

			//std::string flag = "";
			//do
			//{
			//	std::cout << current_dir->GetPath() << "\\*, " << "是否确认(Y/N)? ";
			//	std::getline(std::cin, flag);
			//	boost::to_upper(flag);
			//} while (flag[0] != 'N' && flag[0] != 'Y');
			//if (flag[0] == 'N')
			//{
			//	// 取消操作
			//	std::cout << std::endl;
			//	return;
			//}
			//else
			//{
			//	this->MyDelete(current_dir);
			//	std::cout << std::endl;
			//}
		}
		else
		{
			// 最终结果是文件还是目录
			auto all_children = current_dir->GetChildren();

			auto all_file_name = current_dir->GetFileChildrenNameList();

			std::vector<std::string> target_files;

			auto regex = path_list.back();
			boost::to_upper(regex);

			for (auto file_name : all_file_name)
			{
				boost::to_upper(file_name);
				if (Utils::WildCardMatching(file_name, regex))
				{
					target_files.emplace_back(file_name);
				}
			}

			auto child_iter = all_children.find(boost::to_upper_copy(path_list.back()));

			if (target_files.size() == 0)
			{
				std::cout << "找不到 " << current_dir->GenerateDirectPath() << "\\" << path_list.back() << std::endl;
				std::cout << std::endl;
				return;
			}
			else
			{
				for (auto del_name : target_files)
				{
					auto delete_iter = all_children.find(boost::to_upper_copy(del_name));
					auto del_file = static_cast<MyFile*>(delete_iter->second);
					run_delete(del_file);
				}
				std::cout << std::endl;
			}

			if (!target_files.size() && child_iter == all_children.end())
			{
				std::cout << "找不到 " << current_dir->GenerateDirectPath() << "\\" << path_list.back() << std::endl;
				std::cout << std::endl;
				return;
			}
			else
			{
				if (!target_files.size() && child_iter->second->GetType() == FileType::DIR)
				{
					// 是目录 删除该目录下所有文件
					auto del_dir = current_dir->GetDirChildren().find(boost::to_upper_copy(path_list.back()))->second;
					run_delete(del_dir);
					//std::string flag = "";
					//do
					//{
					//	std::cout << child_iter->second->GenerateDirectPath() << "\\*, " << "是否确认(Y/N)? ";
					//	std::getline(std::cin, flag);
					//	boost::to_upper(flag);
					//} while (flag[0] != 'N' && flag[0] != 'Y');
					//if (flag[0] == 'N')
					//{
					//	// 取消操作
					//	std::cout << std::endl;
					//	return;
					//}
					//else
					//{
					//	// TODO
					//	// 删除目录下所有文件
					//	auto del_dir = current_dir->GetDirChildren().find(boost::to_upper_copy(path_list.back()))->second;
					//	this->MyDelete(del_dir);
					//	std::cout << std::endl;

					//}
				}
				else if (!target_files.size() && child_iter->second->GetType() == FileType::SYMLINKD)
				{
					auto _linkd = static_cast<MyLinkDir*>(child_iter->second);

					// 是目录链接 删除该链接到的目录下所有文件
					auto del_dir = _linkd->GetLinkDir().GetDirChildren().find(boost::to_upper_copy(path_list.back()))->second;
					run_delete(del_dir);

					//std::string flag = "";
					//do
					//{
					//	std::cout << _linkd->GetLinkDir().GenerateDirectPath() << "\\*, " << "是否确认(Y/N)? ";
					//	std::getline(std::cin, flag);
					//	boost::to_upper(flag);
					//} while (flag[0] != 'N' && flag[0] != 'Y');
					//if (flag[0] == 'N')
					//{
					//	// 取消操作
					//	std::cout << std::endl;
					//	return;
					//}
					//else
					//{
					//	// TODO
					//	// 删除目录下所有文件
					//	auto del_dir = _linkd->GetLinkDir().GetDirChildren().find(boost::to_upper_copy(path_list.back()))->second;
					//	this->MyDelete(del_dir);
					//	std::cout << std::endl;
					//}
				}
			}
		}
	}
	else if (command_parameters.size() == 2)
	{
		auto path_list = Utils::GetSplitPath(command_parameters[1]);

		// 是目录
		current_dir = Utils::GetPathDir(path_list, false);
		if (current_dir == nullptr)
		{
			std::cout << std::endl;
			return;
		}

		if (current_dir == nullptr)
		{
			std::cout << "找不到 " << current_dir->GenerateDirectPath() << "\\" << path_list.back() << std::endl;
			std::cout << std::endl;
			return;
		}

		run_delete_recursion(current_dir);

		//std::string flag = "";
		//do
		//{
		//	std::cout << current_dir->GenerateDirectPath() << path_list.back() << "\\*, " << "是否确认(Y/N)? ";
		//	std::getline(std::cin, flag);
		//	boost::to_upper(flag);
		//} while (flag[0] != 'N' && flag[0] != 'Y');
		//if (flag[0] == 'N')
		//{
		//	// 取消操作
		//	std::cout << std::endl;
		//	return;
		//}
		//else
		//{
		//	current_dir->DeleteFileInDirRecursion(current_dir);
		//	std::cout << std::endl;
		//}
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
	else if (command_parameters.size() == 2)
	{
		if (command_parameters[0] != "/s")
		{
			std::cout << "命令语法不正确。" << std::endl;
			std::cout << std::endl;
			return false;
		}
	}
	return true;
}
