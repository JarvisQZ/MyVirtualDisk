#pragma once

enum class FileType
{
	DIR,
	OTHER
};

class MyFileBase
{
public:
	MyFileBase();
	virtual ~MyFileBase();

protected:
	std::size_t m_size;
	std::string m_name;
	std::string m_path;
	FileType m_type;

	std::string m_last_modified_date;
	std::string m_last_modified_time;

	std::unique_ptr<MyFileBase> m_parent_dir;
};
