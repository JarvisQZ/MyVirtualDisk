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
			all_file_name = current_dir->GetFileChildrenNameList();
		}
		else
		{
			//遍历src_到指定文件夹
			current_dir = Utils::GetPathDir(path_list, false);

			if (current_dir == nullptr)
			{
				std::cout << "系统找不到指定的路径。" << std::endl;
				std::cout <<
					"已复制" <<
					std::setfill(' ') <<
					std::setw(10) <<
					0 <<
					" 个文件" << std::endl;
				std::cout << std::endl;
				return;
			}

			all_file_name = current_dir->GetFileChildrenNameList();

			regex = path_list.back();
		}

		for (auto file_name : all_file_name)
		{
			if (Utils::WildCardMatching(file_name, regex))
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
			dst_dir = Utils::GetPathDir(path_list, true);
			if (dst_dir == nullptr)
			{
				return;
			}
			new_file_name = path_list.back();
			auto file_name_upper = boost::to_upper_copy(new_file_name);
			auto dst_files = dst_dir->GetChildren();

			// 如果src是文件且存在重名文件
			if (dst_files.find(file_name_upper) != dst_files.end())
			{
				std::cout << "子目录或文件 " << new_file_name << " 已经存在。" << std::endl;
				std::cout << std::endl;
				return;
			}

			// 新建文件

			MyFile * new_file = new MyFile(*static_cast<MyFile*>(current_dir->GetChildren()[file_name_upper]));
			new_file->SetPath(dst_dir->GenerateDirectPath() + "\\" + dst_dir->GetName());
			new_file->SetParentDir(dst_dir);
			new_file->SetName(new_file_name);
			new_file->SetLastModifiedTime();
			dst_dir->AddChild(new_file_name, new_file);

			// 创建成功
			std::cout <<
				"已复制" <<
				std::setfill(' ') <<
				std::setw(10) <<
				1 <<
				" 个文件" << std::endl;
			std::cout << std::endl;

		}
		else
		{
			// 多个文件拷贝，dst应该是目录
			dst_dir = Utils::GetPathDir(path_list, false);
			if (dst_dir == nullptr)
			{
				std::cout << "系统找不到指定的路径。" << std::endl;
				std::cout <<
					"已复制" <<
					std::setfill(' ') <<
					std::setw(10) <<
					0 <<
					" 个文件" << std::endl;
				std::cout << std::endl;
				return;
			}

			// 复制
			auto dst_files = dst_dir->GetChildren();
			long long i = 0;
			for (auto file : this->m_target_files)
			{

				auto new_file_name = boost::to_upper_copy(file);
				// 如果src是文件且存在重名文件
				if (dst_files.find(new_file_name) != dst_files.end())
				{
					std::cout << "子目录或文件 " << file << " 已经存在。" << std::endl;
					continue;
				}

				// 新建文件
				MyFile * new_file = new MyFile(*static_cast<MyFile*>(current_dir->GetChildren()[new_file_name]));
				new_file->SetPath(dst_dir->GenerateDirectPath() + "\\" + dst_dir->GetName());
				new_file->SetParentDir(dst_dir);
				new_file->SetName(file);
				new_file->SetLastModifiedTime();
				dst_dir->AddChild(file, new_file);
				++i;
			}
			// 创建成功
			std::cout <<
				"已复制" <<
				std::setfill(' ') <<
				std::setw(10) <<
				i <<
				" 个文件" << std::endl;
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
			dst_dir = Utils::GetPathDir(dst_path_list, true);
			new_file_name = dst_path_list.back();
			if (dst_dir == nullptr)
			{
				return;
			}
			auto dst_files = dst_dir->GetFileChildren();

			auto file_name_upper = boost::to_upper_copy(new_file_name);
			// 如果src是文件且存在重名文件
			if (dst_files.find(file_name_upper) != dst_files.end())
			{
				std::cout << "子目录或文件 " << new_file_name << " 已经存在。" << std::endl;
				std::cout << std::endl;
				return;
			}

			char* buffer;
			std::size_t size;

			std::ifstream fin(this->m_target_dir + this->m_target_files.back(), std::ios::in | std::ios::binary | std::ios::ate);
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
			dst_dir->AddChild(new_file_name, new_file);

			buffer = nullptr;

			// 创建成功
			std::cout <<
				"已复制" <<
				std::setfill(' ') <<
				std::setw(10) <<
				1 <<
				" 个文件" << std::endl;
			std::cout << std::endl;
		}
		else
		{
			// 多个文件拷贝，dst因该是目录
			dst_dir = Utils::GetPathDir(dst_path_list, false);
			if (dst_dir == nullptr)
			{
				std::cout << std::endl;
				return;
			}
			char *buffer;
			std::size_t size;
			std::string path;
			auto dst_files = dst_dir->GetFileChildren();

			// 每打开一个文件，将它按字节读取到内存
			long long i = 0;
			for (auto file : this->m_target_files)
			{
				auto new_file_name = boost::to_upper_copy(file);
				// 如果src是文件且存在重名文件
				if (dst_files.find(new_file_name) != dst_files.end())
				{
					std::cout << "子目录或文件 " << file << " 已经存在。" << std::endl;
					continue;
				}

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
				dst_dir->AddChild(file, new_file);
				++i;

				buffer = nullptr;
			}
			// 创建成功
			std::cout <<
				"已复制" <<
				std::setfill(' ') <<
				std::setw(10) <<
				i <<
				" 个文件" << std::endl;
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
		// 从真实磁盘到虚拟磁盘
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
			regex = path_list.back();
			real_path = real_path.substr(0, real_path.size() - regex.size());
			this->m_target_dir = real_path;
			this->GetFileNames(real_path, all_file);
		}

		for (auto file_name : all_file)
		{
			// 如果匹配成功
			if (Utils::WildCardMatching(file_name, regex))
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
