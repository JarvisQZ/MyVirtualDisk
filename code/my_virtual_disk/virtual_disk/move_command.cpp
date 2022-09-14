#include "pch.h"
#include "move_command.h"
#include "utils.h"
#include "Error.h"

MoveCommand::MoveCommand()
{
}

MoveCommand::MoveCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


MoveCommand::~MoveCommand()
{
}

void PrintResult(std::size_t i)
{
	std::cout <<
		"移动了" <<
		std::setfill(' ') <<
		std::setw(10) <<
		i <<
		" 个目录" << std::endl;
	std::cout << std::endl;
}

void MoveCommand::Execute(MyVirtualDisk * virtual_disk)
{
	//std::cout << "MOVE Command!" << std::endl;
	auto command_parameters = this->GetCommandParameters();
	auto src_path_list = Utils::GetSplitPath(this->m_src_path);
	auto dst_path_list = Utils::GetSplitPath(this->m_dst_path);
	auto src_name = src_path_list.back();
	auto src_name_upper = boost::to_upper_copy(src_name);
	auto dst_name = dst_path_list.back();
	auto dst_name_upper = boost::to_upper_copy(dst_name);
	// 假设是文件，找到上一级目录
	auto src_dir = Utils::GetPathDir(src_path_list, true);
	auto dst_dir = Utils::GetPathDir(dst_path_list, true);

	if (src_dir == nullptr || dst_dir == nullptr)
	{
		Error::PrintErr(Error::FileNotFoundErr);
		return;
	}

	// 0 无
	// 1 DIR
	// 2 FILE
	// 3 Link
	int src_flag = Utils::IsTargetInDir(src_dir, src_path_list.back());
	int dst_flag = Utils::IsTargetInDir(dst_dir, dst_path_list.back());
	if (!src_flag)
	{
		Error::PrintErr(Error::FileNotFoundErr);
		std::cout << std::endl;
		return;
	}

	if (command_parameters.size() == 1)
	{
		PrintResult(1);
		return;
	}
	std::string err;
	if (this->m_is_cover)
	{
		auto src_ = src_dir->GetChildren().find(src_name_upper)->second;
		src_->MyMove(dst_name, dst_dir, true, err);
		if (!err.empty())
		{
			Error::PrintErr(err);
			return;
		}
		//if (dst_flag == 0)
		//{
		//	// 目标是空，直接修改指针指向
		//	src_dir->GetChildren().find(src_name_upper)->second->SetName(dst_name);
		//	// 在目标文件夹下添加源文件
		//	dst_dir->AddChild(src_dir->GetChildren().find(src_name_upper)->second);
		//	// 在源文件夹删除
		//	src_dir->DeleteChild(src_name);
		//}
		//else if (dst_flag == 1)
		//{
		//	dst_dir = dst_dir->GetDirChildren().find(dst_name_upper)->second;
		//	auto dst_dir_children = dst_dir->GetChildren();
		//	if (dst_dir_children.find(src_name_upper) == dst_dir_children.end())
		//	{
		//		// 目标是空，直接修改指针指向
		//		// 在目标文件夹下添加源文件
		//		dst_dir->AddChild(src_dir->GetChildren().find(src_name_upper)->second);
		//		// 在源文件夹删除
		//		src_dir->DeleteChild(src_name);
		//	}
		//	else
		//	{
		//		// 覆盖目标文件夹下的同名文件，就是删除
		//		delete dst_dir->GetChildren().find(src_name_upper)->second;
		//		dst_dir->DeleteChild(src_name_upper);
		//		// 在目标文件夹下添加源文件
		//		dst_dir->AddChild(src_dir->GetChildren().find(src_name_upper)->second);
		//		// 在源文件夹删除
		//		src_dir->DeleteChild(src_name_upper);
		//	}
		//}
		//else if (dst_flag == 2)
		//{
		//	// 原文件改名
		//	src_dir->GetChildren().find(src_name_upper)->second->SetName(dst_name);
		//	// 覆盖目标文件夹下的同名文件，就是删除
		//	delete dst_dir->GetChildren().find(dst_name_upper)->second;
		//	dst_dir->DeleteChild(dst_name_upper);
		//	// 在目标文件夹下添加源文件
		//	dst_dir->AddChild(src_dir->GetChildren().find(src_name_upper)->second);
		//	// 在源文件夹删除
		//	src_dir->DeleteChild(src_name_upper);
		//}
		//else if (dst_flag == 3)
		//{
		//	// TODO
		//}
	}
	else
	{
		std::string flag = "";

		if (dst_flag == 0)
		{
			// 目标是空，直接修改指针指向
			src_dir->GetChildren().find(src_name_upper)->second->SetName(dst_name);
			// 在目标文件夹下添加源文件
			dst_dir->AddChild(src_dir->GetChildren().find(src_name_upper)->second);
			// 在源文件夹删除
			src_dir->DeleteChild(src_name_upper);
		}
		else if (dst_flag == 1) // DIR
		{
			dst_dir = dst_dir->GetDirChildren().find(dst_name_upper)->second;
			auto dst_dir_children = dst_dir->GetChildren();
			if (dst_dir_children.find(src_name_upper) == dst_dir_children.end())
			{
				// 目标是空，直接修改指针指向
				// 在目标文件夹下添加源文件
				dst_dir->AddChild(src_dir->GetChildren().find(src_name_upper)->second);
				// 在源文件夹删除
				src_dir->DeleteChild(src_name);
			}
			else
			{
				do
				{
					std::cout << "覆盖" << dst_dir_children.find(src_name_upper)->second->GenerateDirectPath() + "/" + dst_name << "吗?(Yes/No): ";
					std::getline(std::cin, flag);
					boost::to_upper(flag);
				} while (flag[0] != 'N' && flag[0] != 'Y');
				if (flag[0] == 'N')
				{
					// 取消操作
					std::cout << std::endl;
					return;
				}
				// 覆盖目标文件夹下的同名文件，就是删除
				delete dst_dir->GetChildren().find(src_name_upper)->second;
				dst_dir->DeleteChild(src_name_upper);
				// 在目标文件夹下添加源文件
				dst_dir->AddChild(src_dir->GetChildren().find(src_name_upper)->second);
				// 在源文件夹删除
				src_dir->DeleteChild(src_name);
			}
		}
		else if (dst_flag == 2) // FILE
		{
			
			do
			{
				std::cout << "覆盖" << dst_dir->GenerateDirectPath() + "/" + dst_name << "吗?(Yes/No): ";
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
				// 原文件改名
				src_dir->GetChildren().find(src_name_upper)->second->SetName(dst_name);
				// 覆盖目标文件夹下的同名文件，就是删除
				delete dst_dir->GetChildren().find(dst_name_upper)->second;
				dst_dir->DeleteChild(dst_name_upper);
				// 在目标文件夹下添加源文件
				dst_dir->AddChild(src_dir->GetChildren().find(src_name_upper)->second);
				// 在源文件夹删除
				src_dir->DeleteChild(src_name_upper);
			}

		}
		else if (dst_flag == 3)
		{

		}
	}

	std::cout << std::endl;
}

bool MoveCommand::IsCommandCorrect()
{
	auto command_parameters = this->GetCommandParameters();

	if (command_parameters.size() < 1 or command_parameters.size() > 3)
	{
		Error::PrintErr(Error::InvalidCommandErr);
		return false;
	}

	if (command_parameters.size() == 2)
	{
		if (boost::to_upper_copy(command_parameters[0]) == "/Y")
		{
			Error::PrintErr(Error::InvalidCommandErr);
			return false;
		}
		else if (boost::to_upper_copy(command_parameters[1]) == "/Y")
		{
			Error::PrintErr(Error::InvalidCommandErr);
			return false;
		}
		this->m_src_path = command_parameters[0];
		this->m_dst_path = command_parameters[1];
	}
	else if (command_parameters.size() == 3)
	{
		if (boost::to_upper_copy(command_parameters[0]) != "/Y")
		{
			Error::PrintErr(Error::InvalidCommandErr);
			return false;
		}
		this->m_is_cover = true;
		this->m_src_path = command_parameters[1];
		this->m_dst_path = command_parameters[2];
	}
	return true;
}
