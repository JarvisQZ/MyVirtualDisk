#include "pch.h"
#include "mklink_command.h"
#include "utils.h"
#include "my_link_file.h"
#include "my_link_dir.h"
#include "file_type.h"


MklinkCommand::MklinkCommand()
{
}

MklinkCommand::MklinkCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


MklinkCommand::~MklinkCommand()
{
}

void MklinkCommand::Execute(MyVirtualDisk * virtual_disk)
{
	//std::cout << "MKLINK Command!" << std::endl;

	auto command_parameters = this->GetCommandParameters();
	auto src_path_list = Utils::GetSplitPath(command_parameters[1]);
	auto dst_path_list = Utils::GetSplitPath(command_parameters[0]);
	auto src_name = src_path_list.back();
	auto src_name_upper = boost::to_upper_copy(src_name);
	auto dst_name = dst_path_list.back();
	auto dst_name_upper = boost::to_upper_copy(dst_name);

	auto src_dir = Utils::GetPathDir(src_path_list, true);
	auto dst_dir = Utils::GetPathDir(dst_path_list, true);

	if (src_dir == nullptr || dst_dir == nullptr)
	{
		std::cout << "系统找不到指定的路径。" << std::endl;
		std::cout << std::endl;
		return;
	}

	// 0 无
	// 1 DIR
	// 2 FILE
	// 3 Link
	// 4 LinkD
	int src_flag = Utils::IsTargetInDir(src_dir, src_path_list.back());
	int dst_flag = Utils::IsTargetInDir(dst_dir, dst_path_list.back());

	if (dst_flag)
	{
		std::cout << "当文件已存在时，无法创建该文件。" << std::endl;
		std::cout << std::endl;
		return;
	}

	if (src_flag == 0) // wu
	{
		// 源 不存在，创建一个无效的link
		auto new_link_file = new MyLinkFile(dst_name, nullptr, command_parameters[1]);
		// 添加到目录下
		dst_dir->AddChild(dst_name, new_link_file);
		// 打印信息
		std::cout << "为 " << dst_name << " <<===>> " << src_name << " 创建的符号链接" << std::endl;
		std::cout << std::endl;
		return;
	}
	else if (src_flag == 1) // dir
	{
		// 源 是一个目录，创建一个linkd
		src_dir = src_dir->GetDirChildren().find(src_name_upper)->second;
		auto new_link_dir = new MyLinkDir(src_dir, dst_name, src_dir->GetPath());
		// 添加到目标目录下
		dst_dir->AddChild(dst_name, new_link_dir);
		// 打印信息
		std::cout << "为 " << dst_name << " <<===>> " << src_name << " 创建的符号链接" << std::endl;
		std::cout << std::endl;
		return;
	}
	else if (src_flag == 2) // file
	{
		// 源 file
		auto src_file = src_dir->GetFileChildren().find(src_name_upper)->second;
		auto new_link_file = new MyLinkFile(dst_name, src_file, src_dir->GetPath() + "\\" + src_name);
		// 添加到目录下
		dst_dir->AddChild(dst_name, new_link_file);
		// 打印信息
		std::cout << "为 " << dst_name << " <<===>> " << src_name << " 创建的符号链接" << std::endl;
		std::cout << std::endl;
		return;

	}
	else if (src_flag == 3) // link
	{
		// 源 Link
		auto src_link_file = src_dir->GetLinkFileChildren().find(src_name_upper)->second;
		auto new_link_file = new MyLinkFile(src_link_file, dst_name, src_dir->GetPath() + "\\" + src_name);
		// 添加到目录下
		dst_dir->AddChild(dst_name, new_link_file);
		// 打印信息
		std::cout << "为 " << dst_name << " <<===>> " << src_name << " 创建的符号链接" << std::endl;
		std::cout << std::endl;
		return;
	}
	else if (src_flag == 4) // linkd
	{
		// 源 是一个LinkD，创建一个linkd
		auto src_link_dir = src_dir->GetLinkDirChildren().find(src_name_upper)->second;
		auto new_link_dir = new MyLinkDir(src_link_dir, dst_name, src_link_dir->GetPath());
		// 添加到目标目录下
		dst_dir->AddChild(dst_name, new_link_dir);
		// 打印信息
		std::cout << "为 " << dst_name << " <<===>> " << src_name << " 创建的符号链接" << std::endl;
		std::cout << std::endl;
		return;
	}


}

bool MklinkCommand::IsCommandCorrect()
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
