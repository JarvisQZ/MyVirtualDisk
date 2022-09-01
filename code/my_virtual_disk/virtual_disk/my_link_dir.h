#pragma once
#include "my_file_base.h"
class MyDir;

class MyLinkDir :
	public MyFileBase
{
public:
	MyLinkDir();
	MyLinkDir(MyDir *target_dir, std::string name, std::string link_path);
	MyLinkDir(MyLinkDir *target_dir, std::string name, std::string link_path);
	virtual ~MyLinkDir();

	MyDir &GetLinkDir() const;
	void SetLinkDir(MyDir *target_file);
	std::string GetLinkPath() const;
	void SetLinkPath(std::string path);
private:
	MyDir *m_link;
};

