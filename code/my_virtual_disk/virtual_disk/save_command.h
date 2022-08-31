#pragma once
#include "command.h"
class SaveCommand :
	public Command
{
public:
	SaveCommand();
	SaveCommand(CommandType command_type, std::deque<std::string> command_parameters);
	virtual ~SaveCommand();

	bool IsAbsolutePath(std::string path);

	//void WriteFileStruct(MyFileBase* filestruct, std::ofstream& fout);
	//void WriteFile(MyFile* targetfile, std::ofstream& fout);
	//void WriteDir(MyDir* targetdir, std::ofstream& fout);

	void WriteFileBase(MyFileBase *target_file_base, std::ofstream& fout);
	void WriteFile(MyFile *target_file, std::ofstream& fout);
	void WriteDir(MyDir *target_dir, std::ofstream& fout);
	void Execute(MyVirtualDisk* virtual_disk);
	bool IsCommandCorrect();
};

