#pragma once
#include "my_dir.h"
class MyVirtualDisk
{
public:
	static MyVirtualDisk* GetInstance();
	//static std::mutex mt;

	MyDir* GetCurrentDir();
	void SetCurrentDir(MyDir &current_dir);
	MyDir* GetRootDir();

	bool GetQuitFlag();
	void SetQuitFlag(bool flag);

private:
	bool m_quit_flag;
	MyDir *m_root_dir = nullptr;
	MyDir *m_current_dir = nullptr;
	MyVirtualDisk();
	~MyVirtualDisk();
	static MyVirtualDisk *m_virtual_disk;
	

	//class CGarbo
	//{
	//public:
	//	~CGarbo() {
	//		if (MyVirtualDisk::m_virtual_disk)
	//		{
	//			delete MyVirtualDisk::m_virtual_disk;
	//			MyVirtualDisk::m_virtual_disk = nullptr;
	//		}
	//	}
	//	static CGarbo gggarbo;
	//};
	
};

