#pragma once
#include "my_file_base.h"
class MyDir;

class MyFile
	: public MyFileBase
{
public:
	MyFile();
	MyFile(std::string name, std::string path, MyDir &parent_dir);
	MyFile(const MyFile &obj);
	virtual ~MyFile();

	char* GetContent() const;
	void SetContent(char* content, std::size_t size);


private:
	char *m_content = nullptr;
};

