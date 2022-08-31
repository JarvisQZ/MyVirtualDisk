#include "pch.h"
#include "my_link_file.h"


MyLinkFile::MyLinkFile()
{
}

MyLinkFile::MyLinkFile(MyFile * target_file, FileType link_type, std::string link_path)
	:m_link(target_file), m_link_type(link_type), m_link_path(link_path)
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
	this->m_link_path = target_file->GenerateDirectPath();
}

std::string MyLinkFile::GetLinkPath() const
{
	return this->m_link_path;
}

void MyLinkFile::SetLinkPath(std::string path)
{
	this->m_link_path = path;
}

FileType MyLinkFile::GetLinkType() const
{
	return this->m_link_type;
}

std::string MyLinkFile::GetTypeToString() const
{
	return "<SYMLINK>";
}

FileType MyLinkFile::GetType() const
{
	return this->m_link_type;
}
