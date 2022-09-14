#include "pch.h"
#include "utils.h"
#include "file_type.h"
#include "ren_command.h"
#include "Error.h"


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
	auto command_parameters = this->GetCommandParameters();
	auto src_ = command_parameters[0];
	auto dst_ = command_parameters[1];

	auto src_path_list = Utils::GetSplitPath(src_);
	auto dst_path_list = Utils::GetSplitPath(dst_);

	std::string err;

	// 转到指定目录
	auto target_dir = Utils::GetPathDir(src_path_list, true);
	if (!target_dir)
	{
		return;
	}

	if (dst_path_list.size() != 1 or Utils::IsNameIllegal(dst_))
	{
		err = Error::InvalidNameErr;
		Error::PrintErr(err);
		return;
	}

	// 文件是否存在
	auto child = target_dir->FindChild(src_path_list.back());
	if (!child)
	{
		err = Error::FileNotFoundErr;
		Error::PrintErr(err);
		return;
	}
	// 改名
	child->MyRename(dst_, err);
	if (!err.empty())
	{
		Error::PrintErr(err);
	}
	else
	{
		std::cout << std::endl;
	}
}

bool RenCommand::IsCommandCorrect()
{
	if (this->GetCommandParameters().size() != 2)
	{
		Error::PrintErr(Error::InvalidCommandErr);
		return false;
	}
	return true;
}
