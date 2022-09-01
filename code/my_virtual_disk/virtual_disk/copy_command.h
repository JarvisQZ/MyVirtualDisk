#pragma once
#include "command.h"
class CopyCommand :
	public Command
{
public:
	CopyCommand();
	CopyCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~CopyCommand();

	//获取磁盘目录下所有文件的名字，返回状态
	bool GetFileNames(std::string path, std::vector<std::string>& files);

	//返回目录指针，如果type=true，则path必须是一个文件夹路径，如果type=false，给定的path是一个文件路径，返回文件父目录
	//MyDir* GetPathDir(MyVirtualDisk* virtualdisk, std::string path, bool type);

	//MyDir *GetPathDir(MyVirtualDisk* virtualdisk, std::vector<std::string> path_list);

	// 根据路径判断文件是否存在，返回文件类型
	// 0 err 未找到
	// 1 File
	// 2 Dir
	int FindFileBase(std::string path);

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();

private:
	// 目标目录
	std::string m_target_dir = "";
	// 拷贝类型
	// 0 虚拟磁盘到虚拟磁盘
	// 1 真实磁盘到虚拟磁盘
	int m_copy_type = 0;
	// 需要拷贝的文件列表
	std::vector<std::string> m_target_files;
};

