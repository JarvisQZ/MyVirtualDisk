#include "pch.h"
#include "save_command.h"
#include "my_file_base.h"
#include "my_file.h"
#include "my_dir.h"
#include "file_type.h"
#include "utils.h"


SaveCommand::SaveCommand()
{
}

SaveCommand::SaveCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


SaveCommand::~SaveCommand()
{
}

bool SaveCommand::IsAbsolutePath(std::string path)
{
	return false;
}

void SaveCommand::WriteFileBase(MyFileBase* target_file_base, std::ofstream& fout)
{
	std::size_t len, add_tail_len;
	std::string temp_content;

	//获取大小
	temp_content = std::to_string(target_file_base->GetSize());
	len = temp_content.length();
	add_tail_len = len + 1;
	//cout << len << "|3048" << tempcontent<< "|"<<endl;
	fout.write((char*)&len, sizeof(std::size_t)); //将string对象的长度写入文件中
	fout.write(temp_content.data(), add_tail_len);  //将string对象的内容写入文件中

	//获取名字
	temp_content = target_file_base->GetName();
	len = temp_content.length();
	add_tail_len = len + 1;
	//cout << len << "|3053" << tempcontent << "|" << endl;
	fout.write((char*)&len, sizeof(std::size_t)); //将string对象的长度写入文件中
	fout.write(temp_content.data(), add_tail_len);  //将string对象的内容写入文件中

	//获取路径
	temp_content = target_file_base->GetPath();
	len = temp_content.length();
	add_tail_len = len + 1;
	//cout << len << "|3059" << tempcontent << "|" << endl;
	fout.write((char*)&len, sizeof(std::size_t)); //将string对象的长度写入文件中
	fout.write(temp_content.data(), add_tail_len);  //将string对象的内容写入文件中

	//获取修改时间
	temp_content = target_file_base->GetLastModifiedTime();
	len = temp_content.length();
	add_tail_len = len + 1;
	//cout << len << "|3073" << tempcontent << "|" << endl;
	fout.write((char*)&len, sizeof(std::size_t)); //将string对象的长度写入文件中
	fout.write(temp_content.data(), add_tail_len);  //将string对象的内容写入文件中
}

void SaveCommand::WriteFile(MyFile* target_file, std::ofstream& fout)
{
	this->WriteFileBase(target_file, fout);
	//子结构标志
	char* temp_content = target_file->GetContent();
	std::size_t len = target_file->GetSize();
	std::size_t add_taillen = len + 1;
	fout.write((char*)&len, sizeof(std::size_t)); //将string对象的长度写入文件中
	fout.write(temp_content, add_taillen);  //将string对象的内容写入文件中
}
void SaveCommand::WriteDir(MyDir* target_dir, std::ofstream& fout)
{
	this->WriteFileBase(target_dir, fout);
	std::size_t len, add_tail_len;
	std::string temp_content;

	//不为空
	auto all_children = target_dir->GetChildren();
	if (all_children.size() != 0)
	{
		//子结构开始标志
		temp_content = "++++++++++";
		len = temp_content.length();
		add_tail_len = len + 1;
		fout.write((char*)&len, sizeof(std::size_t)); //将string对象的长度写入文件中
		fout.write(temp_content.data(), add_tail_len);  //将string对象的内容写入文件中


		for (auto child : all_children)
		{
			if (child.second->GetType() == FileType::DIR)
			{
				//目录标识符
				temp_content = "**********";
				len = temp_content.length();
				add_tail_len = len + 1;
				fout.write((char*)&len, sizeof(std::size_t)); //将string对象的长度写入文件中
				fout.write(temp_content.data(), add_tail_len);  //将string对象的内容写入文件中

				WriteDir(dynamic_cast<MyDir*>(child.second), fout);
			}
			else
			{
				//文件标识符
				temp_content = "//////////";
				len = temp_content.length();
				add_tail_len = len + 1;
				fout.write((char*)&len, sizeof(std::size_t)); //将string对象的长度写入文件中
				fout.write(temp_content.data(), add_tail_len);  //将string对象的内容写入文件中

				WriteFile(dynamic_cast<MyFile*>(child.second), fout);
			}
		}
		//子结构结束标志
		temp_content = "----------";
		len = temp_content.length();
		add_tail_len = len + 1;
		fout.write((char*)&len, sizeof(std::size_t)); //将string对象的长度写入文件中
		fout.write(temp_content.data(), add_tail_len);  //将string对象的内容写入文件中
	}
	else
	{
		//子结构结束标志
		temp_content = "----------";
		len = temp_content.length();
		add_tail_len = len + 1;
		fout.write((char*)&len, sizeof(std::size_t)); //将string对象的长度写入文件中
		fout.write(temp_content.data(), add_tail_len);  //将string对象的内容写入文件中
	}
}

void SaveCommand::Execute(MyVirtualDisk * virtual_disk)
{
	//std::cout << "Save Command!" << std::endl;

	auto command_parameters = this->GetCommandParameters();

	// 内容
	std::string write_content;
	std::ofstream fout;

	fout.open(command_parameters[0], std::ios::out | std::ios::binary);

	if (!fout.is_open())
	{
		std::cout << "系统找不到指定的路径。" << std::endl;
		std::cout << std::endl;
		return;
	}

	auto root_dir = virtual_disk->GetRootDir();
	this->WriteDir(root_dir, fout);
	fout.close();
	std::cout << "保存成功！" << std::endl;
	std::cout << std::endl;
	return;
}

bool SaveCommand::IsCommandCorrect()
{
	auto command_parameters = this->GetCommandParameters();

	if (command_parameters.size() != 1)
	{
		std::cout << "命令语法不正确。" << std::endl;
		std::cout << std::endl;
		return false;
	}
	return true;
}
