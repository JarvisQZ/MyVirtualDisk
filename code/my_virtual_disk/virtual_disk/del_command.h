#pragma once
#include "command.h"
class MyFile;

class DelCommand :
	public Command
{
public:
	DelCommand();
	DelCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~DelCommand();

	void MyDelete(MyFile *del_file);
	void MyDelete(MyDir *del_dir);

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

