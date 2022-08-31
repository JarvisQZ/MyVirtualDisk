#pragma once
#include "command.h"
class MoveCommand :
	public Command
{
public:
	MoveCommand();
	MoveCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~MoveCommand();

	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();

private:
	bool m_is_cover = 0;
	std::string m_src_path;
	std::string m_dst_path;
};

