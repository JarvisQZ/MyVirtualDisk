#pragma once
#include "my_file_base.h"
class MyDir
	: public MyFileBase
{
public:
	MyDir();
	MyDir(std::string name, std::string path, FileType file_type, MyDir *parent_dir);
	virtual ~MyDir();

private:
	std::map<std::string, MyFileBase *> m_children;
};

