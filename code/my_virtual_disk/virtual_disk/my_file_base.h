#pragma once
#include "file_type.h"
#include "utils.h"
class MyFileBase
{
public:
	MyFileBase();
	MyFileBase(std::string name, std::string path, FileType file_type);
	virtual ~MyFileBase();

	std::string GetName() const;
	void SetName(std::string name);
	std::string GetPath() const;
	void SetPath(std::string path);
	std::string GetType();
	void SetType(FileType file_type);
	std::size_t GetSize() const;
	void SetSize(std::size_t size);
	std::string GetLastModifiedTime() const;
	void SetLastModifiedTime(std::string last_modified_time);

	virtual MyFileBase *GetParentDir();
	void SetParentDir(MyFileBase *parent_dir);

private:
	std::size_t m_size = 0;
	std::string m_name = "";
	std::string m_path = "";
	FileType m_type = FileType::OTHER;

	std::string m_last_modified_time = Utils::GetNowTimeToString();

	MyFileBase *m_parent_dir = nullptr;
};
