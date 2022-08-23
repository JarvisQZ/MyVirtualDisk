#pragma once
class Command
{
public:
	Command();
	virtual ~Command();

	virtual void Execute() = 0;
	virtual bool IsCommandCorrect() = 0;
	virtual bool IsQuit() { return 0; }
};

