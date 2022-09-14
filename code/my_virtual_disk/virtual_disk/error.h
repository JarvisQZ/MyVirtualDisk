#pragma once
class Error
{
public:
	static void PrintErr(std::string err);
public:
	static std::string FileNotFoundErr;
	static std::string RenameErr;
	static std::string InvalidCommandErr;
	static std::string InvalidNameErr;

};

