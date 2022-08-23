#pragma once
#include "my_dir.h"
class MyVirtualDisk
{
public:
	static MyVirtualDisk* GetInstance();
	static void DestoryInstance();
private:
	MyDir *m_current_dir = nullptr;
	MyDir *m_root_dir = nullptr;
	MyVirtualDisk();
	~MyVirtualDisk();
	static MyVirtualDisk *m_virtual_disk;
};

