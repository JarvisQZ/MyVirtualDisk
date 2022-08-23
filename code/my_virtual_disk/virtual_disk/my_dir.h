#pragma once
#include "my_file_base.h"
class MyDir : public MyFileBase
{
public:
	using MyFileBase::MyFileBase;
	MyDir();
	virtual ~MyDir();

private:
	std::map<std::string, MyFileBase&> m_children;
};

