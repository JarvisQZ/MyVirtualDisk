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

std::deque<std::string> my_split(std::string command)
{
	std::deque<std::string> command_parameters;
	size_t pos;

	// 预处理字符串
	boost::replace_all(command, " ", "\n");
	// 三层for 需要优化
	for (size_t i = 0; i < command.length(); ++i)
	{
		if (command[i] == '"')
		{
			for (size_t j = i+1; j < command.length(); j++)
			{
				if (command[j] == '"')
				{
					for (size_t k = i; k < j; k++)
					{
						if (command[k] == '\n')
						{
							command[k] = ' ';
						}
					}
				}
			}
		}
	}

	while (1)
	{
		boost::trim(command); // 去除两边空格
		//cout << "--" << CommandOrder << "--" << endl;
		if (command == "")
			break;
		if (command[0] == '"')
		{
			pos = command.find('"', 2);
			if (pos != std::string::npos)
			{
				command_parameters.push_back(command.substr(1, pos - 1));
				command = command.substr(pos + 1, command.length());
			}
		}
		else
		{
			pos = command.find('\n');
			//cout << TempCommandOrder.substr(0, pos) << "-s" << endl;
			auto patameter = boost::replace_all_copy(command.substr(0, pos), "\"", "");
			command_parameters.push_back(patameter);
			if (pos == std::string::npos)
				break;
			command = command.substr(pos, command.length());
		}
	}

	return command_parameters;
}

std::deque<std::string> Utils::GetCommandParameters(std::string command)
{
	std::deque<std::string> command_parameters;

	// 可能的命令：             Cd ././\\.\\.\\.\\.\\../../"中文 空格 目录"/文件    第二个参数
	boost::trim(command); // 去除两边空格
	// 按照空格分割，重复空格算一个空格
	// 但不能支持名字带空格的目录
	//boost::split(command_parameters, command, boost::is_space(), boost::token_compress_on);

	command_parameters = my_split(command);
	if (command_parameters.size() == 0)
	{
		command_parameters.push_back("");
	}
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

// 0 没有，1 目录 2 文件 3 link 4 linkd
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
	else if (child_iter->second->GetType() == FileType::SYMLINKD)
	{
		return 4;
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
			auto name_upper = boost::to_upper_copy(path_list[i]);
			if (all_children.find(name_upper) == all_children.end())
			{
				std::cout << "系统找不到指定的文件。" << std::endl;
				std::cout << std::endl;
				return nullptr;
			}
			else
			{
				// 如果找到文件，输出错误信息
				if (all_children[name_upper]->GetType() == FileType::OTHER)
				{
					std::cout << "系统找不到指定的文件。" << std::endl;
					std::cout << std::endl;
					return nullptr;
				}
				else
				{
					current_dir = children_dir[name_upper];
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

bool Utils::WildCardMatching(std::string path, std::string input_path)
{
	boost::to_upper(path);
	boost::to_upper(input_path);
	auto m = path.size();
	auto n = input_path.size();

	std::vector < std::vector<size_t> > dp(m + 1, std::vector<std::size_t>(n + 1));

	dp[0][0] = true;
	for (size_t i = 1; i <= n; ++i) {
		if (input_path[i - 1] == '*') {
			dp[0][i] = true;
		}
		else {
			break;
		}
	}
	for (size_t i = 1; i <= m; ++i) {
		for (size_t j = 1; j <= n; ++j) {
			if (input_path[j - 1] == '*') {
				dp[i][j] = dp[i][j - 1] | dp[i - 1][j];
			}
			else if (input_path[j - 1] == '?' || path[i - 1] == input_path[j - 1]) {
				dp[i][j] = dp[i - 1][j - 1];
			}
		}
	}
	return dp[m][n];
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
