#include "pch.h"
#include "my_dir.h"
#include "my_virtual_disk.h"
#include "utils.h"
#include "file_type.h"

std::string Utils::GetNowTimeToString()
{
	std::ostringstream buf;
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);

	buf << sys_time.wYear << "/"
		<< std::setfill('0') << std::setw(2) << sys_time.wMonth << "/"
		<< std::setfill('0') << std::setw(2) << sys_time.wDay
		<< "  " // 两个空格
		<< std::setfill('0') << std::setw(2) << sys_time.wHour << ":"
		<< std::setfill('0') << std::setw(2) << sys_time.wMinute;

	return buf.str();
}

//std::string Utils::GetCommand(std::vector<std::string> command_parameters)
//{
//	return command_parameters[0];
//}

std::deque<std::string> Utils::GetCommandParameters(std::string command)
{
	boost::trim(command);
	std::deque<std::string> command_parameters;
	boost::split(command_parameters, command, boost::is_space(), boost::token_compress_on);
	return command_parameters;
}

std::vector<std::string> Utils::GetSplitPath(std::string _path)
{
	// 去除路径右边无用的斜杠
	boost::trim_right_if(_path, boost::is_any_of("/\\"));

	std::vector<std::string> path_list;
	boost::split(path_list, _path, boost::is_any_of("/\\"), boost::token_compress_on);
	return path_list;
}

bool Utils::IsNameIllegal(std::string name)
{
	if (name.find("/") == std::string::npos &&
		name.find("\\") == std::string::npos &&
		name.find("<") == std::string::npos &&
		name.find(">") == std::string::npos &&
		name.find("*") == std::string::npos &&
		name.find("?") == std::string::npos &&
		name.find("|") == std::string::npos &&
		name.size() > 0)
	{
		return false;
	}
	return true;
}

// 0 没有，1 目录 2 文件 3 link
int Utils::IsTargetInDir(MyDir * src_dir, std::string target)
{
	auto all_children = src_dir->GetChildren();

	auto child_iter = all_children.find(boost::to_upper_copy(target));

	if (child_iter == all_children.end())
	{
		return 0;
	}
	
	if (child_iter->second->GetType() == FileType::DIR)
	{
		return 1;
	}
	else if (child_iter->second->GetType() == FileType::OTHER)
	{
		return 2;
	}
	else if (child_iter->second->GetType() == FileType::SYMLINK)
	{
		return 3;
	}
	
}

MyDir * Utils::GetPathDir(std::vector<std::string> path_list, bool is_file)
{

	auto virtual_disk = MyVirtualDisk::GetInstance();
	auto *current_dir = virtual_disk->GetCurrentDir();
	auto children_dir = current_dir->GetDirChildren();
	auto all_children = current_dir->GetChildren();

	for (size_t i = 0; i < path_list.size(); ++i)
	{
		if (i == path_list.size() - 1)
		{
			if (is_file)
			{
				break;
			}
		}
		// 绝对路径
		if (i == 0 and (path_list[i] == "C:" or path_list[0][0] == '/'))
		{
			//TODO 根目录情况
			current_dir = virtual_disk->GetRootDir();
			children_dir = current_dir->GetDirChildren();
		}
		else if (path_list[i] == "..")
		{
			current_dir = current_dir->GetParentDir();
			children_dir = current_dir->GetDirChildren();
		}
		else if (path_list[i] == ".")
		{
			continue;
		}
		else
		{
			all_children = current_dir->GetChildren();
			if (all_children.find(path_list[i]) == all_children.end())
			{
				std::cout << "系统找不到指定的文件。" << std::endl;
				std::cout << std::endl;
				return nullptr;
			}
			else
			{
				// 如果找到文件，输出错误信息
				if (all_children[path_list[i]]->GetType() == FileType::OTHER)
				{
					std::cout << "系统找不到指定的文件。" << std::endl;
					std::cout << std::endl;
					return nullptr;
				}
				else
				{
					current_dir = children_dir[path_list[i]];
					children_dir = current_dir->GetDirChildren();
				}
			}
		}
	}

	//all_children = current_dir->GetChildren();
	//if (all_children.find(path_list.back()) == all_children.end())
	//{
	//	std::cout << "系统找不到指定的文件。" << std::endl;
	//	std::cout << std::endl;
	//	return nullptr;
	//}
	//else
	//{
	//	// 如果最后是文件，返回当前目录
	//	// 是目录，返回目录
	//	if (all_children[path_list.back()]->GetType() != FileType::OTHER)
	//	{
	//		current_dir = children_dir[path_list.back()];
	//		children_dir = current_dir->GetDirChildren();
	//	}
	//}

	return current_dir;
}

//std::string Utils::GenerateDirectPath(MyVirtualDisk *virtual_disk)
//{
//	//std::stack<MyFileBase*> s;
//	//std::string result;
//	//auto file_name = file.GetName();
//
//	//while (file.GetParentDir() != virtual_disk->GetRootDir())
//	//{
//	//	s.push(file);
//	//	file = file.GetParentDir();
//	//}
//
//	//s.push(virtual_disk->GetRootDir());
//
//	//if (s.size() == 1)
//	//{
//	//	result = s.top()->GetName() + "\\";
//	//}
//	//else
//	//{
//	//	result = boost::join(s, "\\");
//	//}
//
//	return std::string();
//}
