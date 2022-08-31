#pragma once
#include "my_dir.h"
class MyLinkDir :
	public MyDir
{
public:
	MyLinkDir();
	virtual ~MyLinkDir();
private:
	MyDir *m_link;
};

