#include "pch.h"
#include "my_dir.h"


MyDir::MyDir()
	:MyFileBase()
{
}

MyDir::MyDir(std::string name, std::string path, FileType file_type, MyDir *parent_dir)
	: MyFileBase(name, path, file_type)
{
	if (parent_dir == nullptr)
	{
		this->SetParentDir(this);
	}
	else
	{
		this->SetParentDir(parent_dir);
	}
}


MyDir::~MyDir()
{
	std::map<std::string, MyFileBase *>().swap(this->m_children);
}

std::map<std::string, MyFileBase*> MyDir::GetChildren()
{
	return this->m_children;
}

std::map<std::string, MyFileBase*> MyDir::GetDirChildren()
{
	std::map<std::string, MyFileBase*> dir_children;

	for (auto children : this->GetChildren())
	{
		if (children.second->GetTypeToString() == "<DIR>")
		{
			dir_children.insert_or_assign(children.first, children.second);
		}
	}

	return dir_children;
}

MyDir *MyDir::GetParentDir()
{
	return dynamic_cast<MyDir*>(this->MyFileBase::GetParentDir());
}

void MyDir::PrintFileAndDir()
{
	std::size_t file_num = 0;
	std::size_t all_file_size = 0;

	if (this->GetParentDir() != this)
	{
		std::cout << this->GetLastModifiedTime() << this->GetTypeToString() << "    " << std::setw(10) << this->GetSize() << " ." << std::endl;
		std::cout << this->GetLastModifiedTime() << this->GetTypeToString() << "    " << std::setw(10) << this->GetSize() << " .." << std::endl;
	}

	std::cout << std::endl;
	std::cout << " " << this->GetPath() << " 的目录" << std::endl;
	std::cout << std::endl;

	std::cout << this->GetLastModifiedTime()
		<< std::setfill(' ') << std::setw(9) << this->GetTypeToString()
		<< std::setfill(' ') << std::setw(10) << ""
		<< " ." << std::endl;

	std::cout << this->GetLastModifiedTime()
		<< std::setfill(' ') << std::setw(9) << this->GetTypeToString()
		<< std::setfill(' ') << std::setw(10) << ""
		<< " .." << std::endl;

	for (auto child : this->GetChildren())
	{
		if (child.second->GetTypeToString() != "<DIR>")
		{
			file_num++;
			all_file_size += child.second->GetSize();
		}
		std::cout << child.second->GetLastModifiedTime()
			<< std::setfill(' ') << std::setw(9) << child.second->GetTypeToString()
			<< std::setfill(' ') << std::setw(10) << child.second->GetSize()
			<< " " << child.second->GetName() << std::endl;
	}
	std::cout << std::setfill(' ') << std::setw(16) << file_num << " 个文件"
		<< std::setfill(' ') << std::setw(16) << all_file_size << " 字节" << std::endl << std::endl;
}

void MyDir::CreateFileOrDir(std::string name, MyFileBase * new_file)
{
	this->m_children[name] = new_file;
	this->SetLastModifiedTime(Utils::GetNowTimeToString());
	this->UpdateDirSizeUpward();
}

void MyDir::UpdateDirSizeUpward()
{
	MyDir* dir_ptr = nullptr;
	MyDir* old_dir = nullptr;

	while (dir_ptr != old_dir)
	{
		std::size_t size = 0;
		for (auto f : dir_ptr->GetChildren())
		{
			size += f.second->GetSize();
		}
		dir_ptr->SetSize(size);
		old_dir = dir_ptr;
		dir_ptr = dynamic_cast<MyDir*>(dir_ptr->MyFileBase::GetParentDir());
	}
}

