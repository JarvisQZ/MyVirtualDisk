#include "pch.h"
#include "my_link_file.h"
#include "file_type.h"
#include "my_file.h"


MyLinkFile::MyLinkFile()
{
}

MyLinkFile::MyLinkFile(std::string link_name, MyFile * target_file, std::string link_path)
	:MyFileBase(link_name, link_path, FileType::SYMLINK),
	m_link(target_file)
{
}

MyLinkFile::MyLinkFile(MyLinkFile * target_file, std::string name, std::string link_path)
	: MyFileBase(name, link_path, FileType::SYMLINK),
	m_link(&target_file->GetLinkFile())
{
}


MyLinkFile::~MyLinkFile()
{
	this->m_link = nullptr;
}

MyFile & MyLinkFile::GetLinkFile() const
{
	return *this->m_link;
}

void MyLinkFile::SetLinkFile(MyFile * target_file)
{
	this->m_link = target_file;
}

std::string MyLinkFile::GetLinkPath() const
{
	return this->m_link->GetPath();
}

void MyLinkFile::SetLinkPath(std::string path)
{
	this->m_link->SetPath(path);
}
