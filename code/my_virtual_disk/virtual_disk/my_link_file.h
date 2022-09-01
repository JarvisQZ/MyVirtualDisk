#pragma once
#include "my_file_base.h"
class MyFile;

class MyLinkFile :
	public MyFileBase
{
public:
	MyLinkFile();
	MyLinkFile(MyFile *target_file,std::string name, std::string link_path);
	virtual ~MyLinkFile();

	MyFile &GetLinkFile() const;
	void SetLinkFile(MyFile *target_file);
	std::string GetLinkPath() const;
	void SetLinkPath(std::string path);

private:
	MyFile *m_link = nullptr;
};

