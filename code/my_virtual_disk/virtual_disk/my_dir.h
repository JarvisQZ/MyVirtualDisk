#pragma once
#include "my_file_base.h"
class MyDir
	: public MyFileBase
{
public:
	MyDir();
	MyDir(std::string name, std::string path, FileType file_type, MyDir *parent_dir);
	virtual ~MyDir();

	std::map<std::string, MyFileBase *> GetChildren();
	std::map<std::string, MyDir*> GetDirChildren();

	virtual MyDir * GetParentDir() override;

	void PrintFileAndDir(bool flag);

	void PrintFileAndDirRecursion(MyDir* current_dir);

	void CreateFileOrDir(std::string name, MyFileBase* new_file);

	void DeleteChild(std::string name);

	void UpdateDirSizeUpward();

private:
	std::map<std::string, MyFileBase *> m_children;
};

