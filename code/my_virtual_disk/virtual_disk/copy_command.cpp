#include "pch.h"
#include "copy_command.h"
#include "file_type.h"
#include "my_file_base.h"
#include "my_file.h"
#include "utils.h"


CopyCommand::CopyCommand()
{
}

CopyCommand::CopyCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


CopyCommand::~CopyCommand()
{
	std::vector<std::string>().swap(this->m_target_files);
}

bool CopyCommand::GetFileNames(std::string path, std::vector<std::string>& files)
{
	//文件句柄
	//注意：如果此处是long类型，实测运行中会报错访问异常
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	//string p;p.assign(path).append("/*").c_str()
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,跳过
			//如果不是,把文件绝对路径存入vector中
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				continue;
			}
			else
			{
				files.push_back(fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	else
	{
		//失败
		return false;
	}
	return true;
}

//通配符
bool CopyCommand::WildCardMatching(std::string path, std::string input_path)
{
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

MyDir * CopyCommand::GetPathDir(MyVirtualDisk * virtualdisk, std::string path, bool type)
{
	return nullptr;
}

MyDir * CopyCommand::GetPathDir(MyVirtualDisk * virtual_disk, std::vector<std::string> path_list)
{
	MyDir *current_dir = virtual_disk->GetCurrentDir();
	auto children_dir = current_dir->GetDirChildren();
	auto all_children = current_dir->GetChildren();


	for (size_t i = 0; i < path_list.size() - 1; ++i)
	{
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

	all_children = current_dir->GetChildren();
	if (all_children.find(path_list.back()) == all_children.end())
	{
		//std::cout << "系统找不到指定的文件。" << std::endl;
		//std::cout << std::endl;
		//return nullptr;
	}
	else
	{
		// 如果最后是文件，返回当前目录
		// 是目录，返回目录
		if (all_children[path_list.back()]->GetType() != FileType::OTHER)
		{
			current_dir = children_dir[path_list.back()];
			children_dir = current_dir->GetDirChildren();
		}
	}
	return current_dir;
}

int CopyCommand::FindFileBase(std::string path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		// 存在的目录
		if (s.st_mode & S_IFREG)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	return 0;
}

void CopyCommand::Execute(MyVirtualDisk * virtual_disk)
{
	//std::cout << "COPY Command!" << std::endl;

	auto command_parameters = this->GetCommandParameters();
	auto src_ = command_parameters[0];
	auto dst_ = command_parameters[1];
	MyDir *current_dir = virtual_disk->GetCurrentDir();
	auto children_dir = current_dir->GetDirChildren();

	auto path_list = Utils::GetSplitPath(src_);

	switch (this->m_copy_type)
	{
	case 0: // 从虚拟磁盘到虚拟磁盘
	{
		std::string regex;
		std::vector<std::string> all_file_name;
		if (path_list.size() == 1)// 当前目录
		{
			regex = path_list[0];
			boost::to_upper(regex);
			all_file_name = current_dir->GetFileChildren();
		}
		else
		{
			//遍历src_到指定文件夹
			current_dir = this->GetPathDir(virtual_disk, path_list);

			if (current_dir == nullptr)
			{
				return;
			}

			all_file_name = current_dir->GetFileChildren();

			regex = path_list.back();
			boost::to_upper(regex);
		}

		for (auto file_name : all_file_name)
		{
			if (this->WildCardMatching(file_name, regex))
			{
				this->m_target_files.emplace_back(file_name);
			}
		}
		if (this->m_target_files.size() == 0)
		{
			std::cout << "系统找不到指定的文件。" << std::endl;
			std::cout << std::endl;
			return;
		}

		MyDir * dst_dir;
		path_list = Utils::GetSplitPath(dst_);
		if (this->m_target_files.size() == 1)
		{
			std::string new_file_name;
			dst_dir = this->GetPathDir(virtual_disk, path_list);
			if (dst_dir == nullptr)
			{
				return;
			}
			new_file_name = path_list.back();
			auto dst_files = dst_dir->GetChildren();

			// 如果src是文件且存在重名文件
			if (dst_files.find(new_file_name) != dst_files.end())
			{
				std::cout << "子目录或文件 " << new_file_name << " 已经存在。" << std::endl;
				std::cout << std::endl;
				return;
			}

			// 新建文件

			MyFile * new_file = new MyFile(*static_cast<MyFile*>(current_dir->GetChildren()[this->m_target_files[0]]));
			new_file->SetPath(dst_dir->GenerateDirectPath() + "\\" + dst_dir->GetName());
			new_file->SetParentDir(dst_dir);
			new_file->SetName(new_file_name);
			new_file->SetLastModifiedTime();
			dst_dir->CreateFileOrDir(new_file_name, new_file);

			// 创建成功
			std::cout << std::endl;

		}
		else
		{
			dst_dir = GetPathDir(virtual_disk, path_list);
			if (dst_dir == nullptr)
			{
				std::cout << std::endl;
				return;
			}

			// 复制
			auto dst_files = dst_dir->GetChildren();
			for (auto file : this->m_target_files)
			{
				// 新建文件

				MyFile * new_file = new MyFile(*static_cast<MyFile*>(current_dir->GetChildren()[this->m_target_files[0]]));
				new_file->SetPath(dst_dir->GenerateDirectPath() + "\\" + dst_dir->GetName());
				new_file->SetParentDir(dst_dir);
				new_file->SetName(file);
				new_file->SetLastModifiedTime();
				dst_dir->CreateFileOrDir(file, new_file);
			}
			// 创建成功
			std::cout << std::endl;
		}
		break;
	}


	case 1: // 从真实磁盘到虚拟磁盘
	{
		std::string real_src_ = src_.substr(1, src_.size());
		auto dst_path_list = Utils::GetSplitPath(dst_);
		MyDir *dst_dir;

		if (this->m_target_files.size() == 1)
		{
			std::string new_file_name;
			dst_dir = GetPathDir(virtual_disk, dst_path_list);
			new_file_name = dst_path_list.back();
			if (dst_dir == nullptr)
			{
				return;
			}

			char* buffer;
			std::size_t size;

			std::ifstream fin(this->m_target_dir + this->m_target_files[0], std::ios::in | std::ios::binary | std::ios::ate);
			size = static_cast<size_t>(fin.tellg()) + 1;

			// try
			buffer = new char[size];

			memset(buffer, 0, size);
			fin.seekg(fin.beg);
			fin.read(buffer, size);
			fin.close();

			std::string path;

			path = dst_dir->GenerateDirectPath();
			
			// 新建文件
			MyFile* new_file = new MyFile(new_file_name, path, *dst_dir);

			new_file->SetContent(buffer, size);
			dst_dir->CreateFileOrDir(new_file_name, new_file);

			buffer = nullptr;

			// 创建成功
			std::cout << std::endl;
		}
		else
		{
			dst_dir = GetPathDir(virtual_disk, dst_path_list);
			if (dst_dir == nullptr)
			{
				std::cout << std::endl;
				return;
			}
			char *buffer;
			std::size_t size;
			std::string path;

			// 每打开一个文件，将它按字节读取到内存
			for (auto file : this->m_target_files)
			{
				std::ifstream fin(this->m_target_dir + file, std::ios::in | std::ios::binary | std::ios::ate);
				size = static_cast<size_t>(fin.tellg()) + 1;

				// try
				buffer = new char[size];

				memset(buffer, 0, size);
				fin.seekg(fin.beg);
				fin.read(buffer, size);
				fin.close();

				std::string path;

				path = dst_dir->GenerateDirectPath();

				// 新建文件
				MyFile* new_file = new MyFile(file, path, *dst_dir);

				new_file->SetContent(buffer, size);
				dst_dir->CreateFileOrDir(file, new_file);

				buffer = nullptr;
			}
			// 创建成功
			std::cout << std::endl;
		}
	}
	break;
	default:
		break;
	}
}

bool CopyCommand::IsCommandCorrect()
{
	auto command_parameters = this->GetCommandParameters();

	if (command_parameters.size() != 2)
	{
		return false;
	}

	// 真实路径
	if (command_parameters[0][0] == '@' and command_parameters[1][0] == '@')
	{
		//this->m_copy_type = 2;
		std::cout << "从真实磁盘到真实磁盘，此功能还未实现！" << std::endl;
		std::cout << std::endl;
		return false;
	}
	else if (command_parameters[0][0] == '@' and command_parameters[1][0] != '@')
	{
		std::vector<std::string> all_file;

		auto real_path = command_parameters[0].substr(1, command_parameters[0].size() - 1);

		std::string regex;

		auto path_list = Utils::GetSplitPath(real_path);

		if (path_list.size() == 1)
		{
			regex = real_path;
			GetFileNames(".", all_file);
		}
		else
		{

		}

		for (auto file_name : all_file)
		{
			// 如果匹配成功
			if (WildCardMatching(file_name, regex))
			{
				this->m_target_files.emplace_back(file_name);
			}
		}

		if (this->m_target_files.size() == 0)
		{
			std::cout << "系统找不到指定的文件。" << std::endl;
			std::cout << std::endl;
			return false;
		}

		this->m_copy_type = 1;
		return true;
	}
	else if (command_parameters[0][0] != '@' and command_parameters[1][0] == '@')
	{
		//this->m_copy_type = 3;
		std::cout << "从虚拟磁盘到真实磁盘，此功能还未实现！" << std::endl;
		std::cout << std::endl;
		return false;
	}
	else if (command_parameters[0][0] != '@' and command_parameters[1][0] != '@')
	{
		this->m_copy_type = 0;
	}

	return true;
}
