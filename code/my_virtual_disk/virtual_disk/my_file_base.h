#pragma once
//#include "file_type.h"
enum class FileType;
//#include "my_dir.h"
class MyDir;

class MyFileBase
{
public:
	MyFileBase();
	MyFileBase(std::string name, std::string path, FileType file_type);
	MyFileBase(std::string name, std::string path, FileType file_type, MyDir &parent_dir);
	virtual ~MyFileBase();

	virtual std::string GetName() const;
	void SetName(std::string name);
	std::string GetPath() const;
	std::string GenerateDirectPath();
	void SetPath(std::string path);
	virtual std::string GetTypeToString() const;
	virtual FileType GetType() const;
	void SetType(FileType file_type);
	std::size_t GetSize() const;
	void SetSize(std::size_t size);
	std::string GetLastModifiedTime() const;
	void SetLastModifiedTime();

	MyDir *GetParentDir() const;
	void SetParentDir(MyDir *parent_dir);

private:
	std::size_t m_size = 0;
	std::string m_name = "";
	std::string m_path = "";
	FileType m_type;

	std::string m_last_modified_time  = "";

	MyDir *m_parent_dir = nullptr;
};
