#pragma once
#include "command.h"
class LoadCommand :
	public Command
{
public:
	LoadCommand();
	LoadCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~LoadCommand();

	void ReadFileBase(MyFileBase* file_base, std::ifstream& fin);
	void ReadFile(MyFile* targe_tfile, std::ifstream& fin);
	void ReadDir(MyDir* target_dir, std::ifstream& fin);

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

