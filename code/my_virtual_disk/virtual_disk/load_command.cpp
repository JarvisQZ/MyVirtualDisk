#include "pch.h"
#include "load_command.h"
#include "my_file_base.h"
#include "my_file.h"
#include "my_dir.h"
#include "file_type.h"


LoadCommand::LoadCommand()
{
}

LoadCommand::LoadCommand(CommandType command_type, std::deque<std::string> command_parameters)
	:Command(command_type, command_parameters)
{
}


LoadCommand::~LoadCommand()
{
}

void LoadCommand::ReadFileBase(MyFileBase* target_file_base, std::ifstream& fin)
{
	std::size_t len = 0;

	char* read_content;
	std::string temp_content;

	//读大小
	fin.read((char*)&len, sizeof(std::size_t));
	len++;
	read_content = new char[len];
	fin.read(read_content, len);
	temp_content = read_content;
	target_file_base->SetSize(atoi(temp_content.c_str()));
	delete[]read_content;

	//读名字
	fin.read((char*)&len, sizeof(std::size_t));
	len++;
	read_content = new char[len];
	fin.read(read_content, len);
	temp_content = read_content;
	target_file_base->SetName(temp_content);
	delete[]read_content;

	//读类型
	fin.read((char*)&len, sizeof(std::size_t));
	len++;
	read_content = new char[len];
	fin.read(read_content, len);
	temp_content = read_content;
	target_file_base->SetPath(temp_content);
	delete[]read_content;

	//读创建时间
	fin.read((char*)&len, sizeof(std::size_t));
	len++;
	read_content = new char[len];
	fin.read(read_content, len);
	temp_content = read_content;
	target_file_base->SetLastModifiedTime();
	delete[]read_content;
}
void LoadCommand::ReadFile(MyFile* targetfile, std::ifstream& fin)
{
	this->ReadFileBase(targetfile, fin);
	std::size_t len;
	char* read_content;
	fin.read((char*)&len, sizeof(std::size_t));
	//len++;
	read_content = new char[len];
	fin.read(read_content, len);
	targetfile->SetContent(read_content, len);
	//delete[]Readcontent;
	read_content = nullptr;
}
void LoadCommand::ReadDir(MyDir* targetdir, std::ifstream& fin)
{
	this->ReadFileBase(targetdir, fin);
	std::size_t len;
	char* read_content;
	std::string tempcontent;

	//读大小
	fin.read((char*)&len, sizeof(std::size_t));
	len++;
	//没出错
	if (fin.good() && !fin.eof())
	{
		read_content = new char[len];
		fin.read(read_content, len);
		tempcontent = read_content;
		delete[]read_content;
		//子结构
		if (tempcontent == "++++++++++")
		{
			while (1)
			{
				fin.read((char*)&len, sizeof(std::size_t));
				len++;
				read_content = new char[len];
				fin.read(read_content, len);
				tempcontent = read_content;
				delete[]read_content;
				//文件
				if (tempcontent == "//////////")
				{
					MyFile* newfile = new MyFile("temp", targetdir->GetPath(), *targetdir);
					this->ReadFile(newfile, fin);
					targetdir->AddChild(newfile);
				}
				//目录
				else if (tempcontent == "**********")
				{
					MyDir* newdir = new MyDir("temp", targetdir->GetPath(),FileType::DIR, targetdir);
					this->ReadDir(newdir, fin);
					targetdir->AddChild(newdir);
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			return;
		}
	}
}

void LoadCommand::Execute(MyVirtualDisk * virtual_disk)
{
	//std::cout << "Load Command!" << std::endl;

	auto command_parameters = this->GetCommandParameters();
	std::ifstream fin;

	fin.open(command_parameters[0], std::ios::in | std::ios::binary);

	if (!fin.is_open())
	{
		std::cout << "系统找不到指定的路径。" << std::endl;
		std::cout << std::endl;
		return;
	}

	auto root_dir = virtual_disk->GetRootDir();

	this->ReadDir(root_dir, fin);

	fin.close();
	std::cout << "读取成功" << std::endl;
	std::cout << std::endl;
	return;
}

bool LoadCommand::IsCommandCorrect()
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
