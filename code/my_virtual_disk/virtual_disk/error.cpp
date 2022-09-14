#include "pch.h"
#include "Error.h"

void Error::PrintErr(std::string err)
{
	std::cout << err << std::endl;
	std::cout << std::endl;
}

std::string Error::FileNotFoundErr = "系统找不到指定的文件。";

std::string Error::RenameErr = "存在一个重名文件，或是找不到文件。";

std::string Error::InvalidCommandErr = "命令语法不正确。";

std::string Error::InvalidNameErr = "文件名、目录名或卷标语法不正确。";

