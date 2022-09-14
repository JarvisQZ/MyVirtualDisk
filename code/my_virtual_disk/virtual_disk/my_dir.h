#pragma once
#include "my_file_base.h"
class MyFile;
class MyLinkFile;
class MyLinkDir;

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
	std::map<std::string, MyLinkFile*> GetLinkFileChildren() const;
	std::map<std::string, MyLinkDir*> GetLinkDirChildren() const;
	std::vector<std::string> GetFileChildrenNameList() const;

	void PrintFileAndDir(bool flag);
	void PrintFileAndDirRecursion(MyDir* current_dir);

	void DeleteFileInDir(MyDir* targetdir);
	void DeleteFileInDirRecursion(MyDir* targetdir);

	void DeleteDir(MyDir* targetdir);
	void DeleteDirRecursion(MyDir* targetdir);
	void DeleteDirRecursionForQuit(MyDir* targetdir);

	void AddChild(MyFileBase * new_file);
	void AddChild(std::string name, MyFileBase* new_file);
	void AddChild(std::string name, MyFileBase* new_file, bool is_override);

	virtual void MyDelete(bool is_force) override;

	void DeleteChild(std::string name);

	MyFileBase* FindChild(std::string file_name);

private:
	std::map<std::string, MyFileBase *> m_children;
};

