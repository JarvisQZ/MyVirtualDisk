#include "pch.h"
#include "my_dir.h"


MyDir::MyDir()
	:MyFileBase()
{
}

MyDir::MyDir(std::string name, std::string path, FileType file_type, MyDir *parent_dir)
	:MyFileBase(name, path, file_type)
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

