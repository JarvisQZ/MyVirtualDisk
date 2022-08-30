#include "pch.h"
#include "my_file.h"
#include "my_file_base.h"
#include "file_type.h"

MyFile::MyFile()
{
}

MyFile::MyFile(std::string name, std::string path, MyDir &parent_dir)
	:MyFileBase(name, path, FileType::OTHER, parent_dir)
{
}

// 拷贝构造
MyFile::MyFile(const MyFile &obj)
{
	this->SetName(obj.GetName());
	this->SetType(obj.GetType());
	this->SetPath(obj.GetPath());
	this->SetParentDir(obj.GetParentDir());
	this->SetSize(obj.GetSize());
	this->SetLastModifiedTime();

	this->m_content = obj.m_content;

}


MyFile::~MyFile()
{
	delete[] m_content;
	m_content = nullptr;
}

char* MyFile::GetContent() const
{
	return this->m_content;
}

void MyFile::SetContent(char* content, std::size_t size)
{
	this->m_content = content;
	this->SetSize(size);
}

