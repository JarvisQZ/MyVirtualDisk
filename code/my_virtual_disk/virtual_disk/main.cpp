// virtual_disk.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "my_virtual_disk.h"
#include "command_factory.h"
#include "command_invoker.h"

void run()
{
	// 创建虚拟磁盘对象
	MyVirtualDisk *virtual_disk = MyVirtualDisk::GetInstance();

	// 创建工厂
	//CommandFactory *command_factory = new CommandFactory();
	CommandFactory command_factory = CommandFactory();

	// 创建命令调用
	//CommandInvoker *command_invoker = new CommandInvoker();
	CommandInvoker command_invoker = CommandInvoker();

	std::string input_command = "";
	Command* command = nullptr;

	// 
	while (!virtual_disk->GetQuitFlag())
	{
		std::cout << virtual_disk->GetCurrentDir()->GetPath() << ">";
		std::getline(std::cin, input_command);

		command = command_factory.BuildCommand(input_command, virtual_disk->GetCurrentDir());

		if (command)
		{
			command_invoker.SetCommand(*command);
			command_invoker.RunCommand(virtual_disk);
		}

		delete command;
		command = nullptr;
	}


	// 清理
	//delete command_factory;
	//command_factory = nullptr;

	//delete command_invoker;
	//command_invoker = nullptr;
}

void test()
{
	std::string s = "abcABC:.,/../.,/.,.//\\(*&%&^$*";
	auto upper = boost::to_upper_copy(s);
	std::cout << upper << std::endl;
}

int main()
{
	run();
	//test();
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
