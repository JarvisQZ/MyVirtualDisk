#include "pch.h"
#include "my_link_dir.h"
#include "my_dir.h"
#include "file_type.h"


MyLinkDir::MyLinkDir()
{
}

MyLinkDir::MyLinkDir(MyDir * target_dir, std::string name, std::string link_path)
	:MyFileBase(name, link_path, FileType::SYMLINKD),
	m_link(target_dir)
{
}


MyLinkDir::~MyLinkDir()
{
	this->m_link = nullptr;
}

MyDir & MyLinkDir::GetLinkDir() const
{

	return *this->m_link;
}

void MyLinkDir::SetLinkDir(MyDir * target_file)
{
	this->m_link = target_file;
}

std::string MyLinkDir::GetLinkPath() const
{
	return this->m_link->GetPath();
}

void MyLinkDir::SetLinkPath(std::string path)
{
	this->m_link->SetPath(path);
}
