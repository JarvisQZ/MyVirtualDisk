#pragma once
#include "my_file_base.h"
class MyFile;

class MyDir
	: public MyFileBase
{
public:
	MyDir();
	MyDir(std::string name, std::string path, FileType file_type, MyDir *parent_dir);
	virtual ~MyDir();

	std::map<std::string, MyFileBase *> GetChildren() const;
	std::map<std::string, MyDir*> GetDirChildren() const;
	std::map<std::string, MyFile*> GetFileChildren() const;


	std::vector<std::string> GetFileChildrenNameList() const;

	//virtual MyDir * GetParentDir() override;

	void PrintFileAndDir(bool flag);

	void PrintFileAndDirRecursion(MyDir* current_dir);

	void AddChild(MyFileBase * new_file);
	void AddChild(std::string name, MyFileBase* new_file);
	void AddChild(std::string name, MyFileBase* new_file, bool is_override);

	void DeleteChild(std::string name);

	void UpdateDirSizeUpward();

private:
	std::map<std::string, MyFileBase *> m_children;
};

