#include "pch.h"
#include "my_virtual_disk.h"

MyVirtualDisk* MyVirtualDisk::m_virtual_disk = nullptr;

MyVirtualDisk* MyVirtualDisk::GetInstance()
{
	if (m_virtual_disk == nullptr)
	{
		m_virtual_disk = new MyVirtualDisk();
	}
	return m_virtual_disk;
}

void MyVirtualDisk::DestoryInstance()
{
	if (m_virtual_disk != nullptr)
	{
		delete m_virtual_disk;
	}
	m_virtual_disk = nullptr;
}

MyVirtualDisk::MyVirtualDisk()
{
	m_root_dir = new MyDir();
}

MyVirtualDisk::~MyVirtualDisk()
{
	if (m_virtual_disk != nullptr)
	{
		delete m_virtual_disk;
	}
	m_virtual_disk = nullptr;
}
