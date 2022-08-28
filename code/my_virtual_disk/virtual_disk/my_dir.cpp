#include "pch.h"
#include "my_dir.h"
#include "file_type.h"


MyDir::MyDir()
	:MyFileBase()
{
}

MyDir::MyDir(std::string name, std::string path, FileType file_type, MyDir *parent_dir)
	: MyFileBase(name, path, file_type)
{
	if (parent_dir == nullptr)
	{
		this->SetParentDir(this);
	}
	else
	{
		this->SetParentDir(parent_dir);
	}
}


MyDir::~MyDir()
{
	// TODO map内部的指针未释放内存
	//for (auto &child : m_children)
	//{
	//	if (child.second->GetType == FileType::DIR)
	//	{
	//		auto c = static_cast<MyDir*>(child.second);
	//		if (c->GetChildren())
	//		{

	//		}
	//	}
	//}

	std::map<std::string, MyFileBase *>().swap(this->m_children);
}

std::map<std::string, MyFileBase*> MyDir::GetChildren()
{
	return this->m_children;
}

std::map<std::string, MyDir*> MyDir::GetDirChildren()
{
	std::map<std::string, MyDir*> dir_children;

	for (auto children : this->GetChildren())
	{
		if (children.second->GetTypeToString() == "<DIR>")
		{
			dir_children.insert_or_assign(children.first, static_cast<MyDir*>(children.second));
		}
	}

	return dir_children;
}

MyDir *MyDir::GetParentDir()
{
	return dynamic_cast<MyDir*>(this->MyFileBase::GetParentDir());
}

// flag = 1 打印文件夹和文件
// flag = 0 只打印文件夹
void MyDir::PrintFileAndDir(bool flag)
{
	std::size_t file_num = 0;
	std::size_t dir_num = 0;
	std::size_t all_file_size = 0;

	std::cout << std::endl;
	std::cout << " " << this->GetPath() << " 的目录" << std::endl;
	std::cout << std::endl;

	if (this->GetParentDir() != this)
	{
		std::cout << this->GetLastModifiedTime()
			<< std::setfill(' ') << std::setw(9) << this->GetTypeToString()
			<< std::setfill(' ') << std::setw(10) << ""
			<< " ." << std::endl;

		std::cout << this->GetLastModifiedTime()
			<< std::setfill(' ') << std::setw(9) << this->GetTypeToString()
			<< std::setfill(' ') << std::setw(10) << ""
			<< " .." << std::endl;
	}

	for (auto child : this->GetChildren())
	{
		if (child.second->GetType() == FileType::DIR)
		{
			++dir_num;
		}
		else
		{
			++file_num;
			all_file_size += child.second->GetSize();
		}
		if (flag)
		{
			std::cout << child.second->GetLastModifiedTime()
				<< std::setfill(' ') << std::setw(9) << child.second->GetTypeToString()
				<< std::setfill(' ') << std::setw(10) << ((child.second->GetType() == FileType::DIR) ? "" : std::to_string(child.second->GetSize()))
				<< " " << child.second->GetName() << std::endl;
		}
		else
		{
			if (child.second->GetType() == FileType::DIR)
			{
				std::cout << child.second->GetLastModifiedTime()
					<< std::setfill(' ') << std::setw(9) << child.second->GetTypeToString()
					<< std::setfill(' ') << std::setw(10) << ""
					<< " " << child.second->GetName() << std::endl;
			}
		}
	}
	if (flag)
	{
		std::cout << std::setfill(' ') << std::setw(16) << file_num << " 个文件"
			<< std::setfill(' ') << std::setw(16) << all_file_size << " 字节" << std::endl;
	}

	std::cout << std::setfill(' ') << std::setw(16) << file_num << " 个文件" << std::endl << std::endl;
}

void MyDir::PrintFileAndDirRecursion(MyDir* current_dir)
{
	current_dir->PrintFileAndDir(true);

	auto children_dir = current_dir->GetDirChildren();

	// DFS
	for (auto &&child_dir : children_dir)
	{
		this->PrintFileAndDirRecursion(child_dir.second);
	}
}

void MyDir::CreateFileOrDir(std::string name, MyFileBase * new_file)
{
	boost::to_upper(name);
	this->m_children[name] = new_file;
	this->SetLastModifiedTime(Utils::GetNowTimeToString());
	this->UpdateDirSizeUpward();
}

void MyDir::UpdateDirSizeUpward()
{
	MyDir* dir_ptr = nullptr;
	MyDir* old_dir = nullptr;

	while (dir_ptr != old_dir)
	{
		std::size_t size = 0;
		for (auto f : dir_ptr->GetChildren())
		{
			size += f.second->GetSize();
		}
		dir_ptr->SetSize(size);
		old_dir = dir_ptr;
		dir_ptr = static_cast<MyDir*>(dir_ptr->MyFileBase::GetParentDir());
	}
}

