#pragma once
#include "my_file.h"
class MyLinkFile :
	public MyFile
{
public:
	MyLinkFile();
	MyLinkFile(MyFile *target_file, FileType link_type, std::string link_path);
	virtual ~MyLinkFile();

	MyFile &GetLinkFile() const;
	void SetLinkFile(MyFile *target_file);
	std::string GetLinkPath() const;
	void SetLinkPath(std::string path);
	FileType GetLinkType() const;

	virtual std::string GetTypeToString() const override;
	virtual FileType GetType() const override;

private:
	MyFile *m_link = nullptr;
	std::string m_link_path;
	FileType m_link_type;
};

