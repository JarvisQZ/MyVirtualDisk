#include "pch.h"
#include "my_file_base.h"
#include "utils.h"

MyFileBase::MyFileBase()
{
}

MyFileBase::MyFileBase(std::string name, std::string path, FileType file_type)
	: m_name(name), m_path(path), m_type(file_type)
{
}

MyFileBase::~MyFileBase()
{
}

std::string MyFileBase::GetName() const
{
	return this->m_name;
}

void MyFileBase::SetName(std::string name)
{
	this->m_name = name;
}

std::string MyFileBase::GetPath() const
{
	return this->m_path;
}

void MyFileBase::SetPath(std::string path)
{
	this->m_path = path;
}

std::string MyFileBase::GetType()
{
	return this->m_type == FileType::DIR ? "<DIR>" : "     ";
}

void MyFileBase::SetType(FileType file_type)
{
	this->m_type = file_type;
}

std::size_t MyFileBase::GetSize() const
{
	return this->m_size;
}

void MyFileBase::SetSize(std::size_t size)
{
	this->m_size = size;
}

std::string MyFileBase::GetLastModifiedTime() const
{
	return this->m_last_modified_time;
}

void MyFileBase::SetLastModifiedTime(std::string last_modified_time)
{
	this->m_last_modified_time = last_modified_time;
}

MyFileBase * MyFileBase::GetParentDir()
{
	return this->m_parent_dir;
}

void MyFileBase::SetParentDir(MyFileBase *parent_dir)
{
	this->m_parent_dir = parent_dir;
}
