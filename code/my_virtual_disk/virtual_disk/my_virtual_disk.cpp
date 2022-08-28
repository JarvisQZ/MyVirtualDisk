#include "pch.h"
#include "my_virtual_disk.h"
#include "file_type.h"

MyVirtualDisk* MyVirtualDisk::m_virtual_disk = nullptr;
//MyDir *MyVirtualDisk::m_root_dir = new MyDir("", "C:\\", FileType::DIR, nullptr);
//std::mutex MyVirtualDisk::mt; // c++ 11 静态变量是多线程安全的 无需DCL

MyVirtualDisk* MyVirtualDisk::GetInstance()
{
	//if (m_virtual_disk == nullptr)
	//{
	//	mt.try_lock();
	//	if (m_virtual_disk == nullptr)
	//	{
	//		m_virtual_disk = new MyVirtualDisk();
	//	}
	//	mt.unlock();
	//}
	//return m_virtual_disk;

	static MyVirtualDisk instance;
	return &instance;
}

MyDir * MyVirtualDisk::GetCurrentDir()
{
	return this->m_current_dir;
}

void MyVirtualDisk::SetCurrentDir(MyDir & current_dir)
{
	this->m_current_dir = &current_dir;
}

MyDir * MyVirtualDisk::GetRootDir()
{
	return this->m_root_dir;
}

bool MyVirtualDisk::GetQuitFlag()
{
	return this->m_quit_flag;
}

void MyVirtualDisk::SetQuitFlag(bool flag)
{
	this->m_quit_flag = flag;
}

MyVirtualDisk::MyVirtualDisk()
	:
	m_current_dir(m_root_dir),
	m_root_dir(new MyDir("", "C:\\", FileType::DIR, nullptr)),
	m_quit_flag(false)
{
}

MyVirtualDisk::~MyVirtualDisk()
{

	if (m_current_dir != m_root_dir)
	{
		if (m_root_dir != nullptr)
		{
			delete m_root_dir;
		}
		m_root_dir = nullptr;
	}

	if (m_current_dir != nullptr)
	{
		delete m_current_dir;
	}
	m_current_dir = nullptr;
	m_root_dir = nullptr;

	//if (MyVirtualDisk::m_root_dir != nullptr)
	//{
	//	delete MyVirtualDisk::m_root_dir;
	//}
	//m_root_dir = nullptr;
}
