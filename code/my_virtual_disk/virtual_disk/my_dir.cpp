#include "pch.h"
#include "utils.h"
#include "my_file_base.h"
#include "my_dir.h"
#include "my_file.h"
#include "my_link_file.h"
#include "my_link_dir.h"
#include "my_virtual_disk.h"
#include "file_type.h"
#include "Error.h"


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

std::map<std::string, MyFileBase*> MyDir::GetChildren() const
{
	return this->m_children;
}

std::map<std::string, MyDir*> MyDir::GetDirChildren() const
{
	std::map<std::string, MyDir*> dir_children;

	for (auto children : this->GetChildren())
	{
		if (children.second->GetType() == FileType::DIR)
		{
			dir_children.insert_or_assign(children.first, static_cast<MyDir*>(children.second));
		}
	}

	return dir_children;
}

std::map<std::string, MyFile*> MyDir::GetFileChildren() const
{
	std::map<std::string, MyFile*> file_children;

	for (auto children : this->GetChildren())
	{
		if (children.second->GetType() == FileType::OTHER)
		{
			file_children.insert_or_assign(children.first, static_cast<MyFile*>(children.second));
		}
	}

	return file_children;
}

std::map<std::string, MyLinkFile*> MyDir::GetLinkFileChildren() const
{
	std::map<std::string, MyLinkFile*> link_file_children;

	for (auto children : this->GetChildren())
	{
		if (children.second->GetType() == FileType::SYMLINK)
		{
			link_file_children.insert_or_assign(children.first, static_cast<MyLinkFile*>(children.second));
		}
	}

	return link_file_children;
}

std::map<std::string, MyLinkDir*> MyDir::GetLinkDirChildren() const
{
	std::map<std::string, MyLinkDir*> link_dir_children;

	for (auto children : this->GetChildren())
	{
		if (children.second->GetType() == FileType::SYMLINKD)
		{
			link_dir_children.insert_or_assign(children.first, static_cast<MyLinkDir*>(children.second));
		}
	}

	return link_dir_children;
}

std::vector<std::string> MyDir::GetFileChildrenNameList() const
{
	std::vector<std::string> result;

	for (auto child : this->m_children)
	{
		if (child.second->GetType() == FileType::OTHER or child.second->GetType() == FileType::SYMLINK)
		{
			result.emplace_back(child.second->GetName());
		}
	}

	return result;
}

// flag = 1 打印文件夹和文件
// flag = 0 只打印文件夹
void MyDir::PrintFileAndDir(bool flag)
{
	std::size_t file_num = 0;
	std::size_t dir_num = 0;
	std::size_t all_file_size = 0;

	std::cout << std::endl;
	std::cout << " " << this->GenerateDirectPath() << " 的目录" << std::endl;
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
		if (child.second->GetType() == FileType::DIR || child.second->GetType() == FileType::SYMLINKD)
		{
			++dir_num;
		}
		else
		{
			++file_num;
			if (child.second->GetType() == FileType::OTHER)
			{
				all_file_size += child.second->GetSize();
			}
		}
		if (flag)
		{
			std::cout << child.second->GetLastModifiedTime() << "    ";

			if (child.second->GetType() != FileType::OTHER)
			{
				std::cout << std::setiosflags(std::ios::left);
				std::cout << std::setw(15) << child.second->GetTypeToString();
				std::cout << std::resetiosflags(std::ios::left);
			}
			else
			{
				std::cout << std::setiosflags(std::ios::right);
				std::cout << std::setw(15) << std::to_string(child.second->GetSize());
				std::cout << std::resetiosflags(std::ios::right);
			}

			std::cout << " " << child.second->GetName();

			if (!(child.second->GetType() == FileType::DIR or child.second->GetType() == FileType::OTHER))
			{
				std::cout << " [" + child.second->GetPath() + "]";
			}

			std::cout << std::endl;
		}
		else
		{
			if (child.second->GetType() == FileType::DIR || child.second->GetType() == FileType::SYMLINKD)
			{
				std::cout << child.second->GetLastModifiedTime() << "    ";

				std::cout << std::setiosflags(std::ios::left);
				std::cout << std::setw(15) << child.second->GetTypeToString();
				std::cout << std::resetiosflags(std::ios::left);

				std::cout << " " << child.second->GetName();

				if (!(child.second->GetType() == FileType::DIR or child.second->GetType() == FileType::OTHER))
				{
					std::cout << " [" + child.second->GetPath() + "]";
				}

				std::cout << std::endl;
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

void MyDir::DeleteFileInDir(MyDir* targetdir)
{
	for (auto &file : targetdir->GetChildren())
	{
		if (file.second->GetType() != FileType::DIR)
		{
			// 子指向父目录置空
			file.second->SetParentDir(nullptr);
			delete file.second;
			file.second = nullptr;
			this->m_children.erase(file.first);
		}
	}
}

void MyDir::DeleteFileInDirRecursion(MyDir* targetdir)
{
	for (auto &dir : targetdir->GetChildren())
	{
		if (dir.second->GetType() == FileType::DIR)
		{
			this->DeleteFileInDirRecursion(static_cast<MyDir*>(dir.second));
		}
		else
		{
			dir.second->SetParentDir(nullptr);
			delete dir.second;
			dir.second = nullptr;
			targetdir->m_children.erase(dir.first);
		}
	}
}

void MyDir::DeleteDir(MyDir * targetdir)
{
	auto virtual_disk = MyVirtualDisk::GetInstance();

	// 要删除的目录非空，返回错误信息
	if (targetdir->GetChildren().size() != 0)
	{
		std::cout << "目录不是空的。" << std::endl;
		std::cout << std::endl;
		return;
	}
	if (targetdir == virtual_disk->GetCurrentDir())
	{
		std::cout << "另一个程序正在使用此文件，进程无法访问。" << std::endl;
		std::cout << std::endl;
		return;
	}
	auto parent_dir = targetdir->GetParentDir();
	parent_dir->DeleteChild(targetdir->GetName());
	delete targetdir;
}

void MyDir::DeleteDirRecursion(MyDir * targetdir)
{
	auto virtual_disk = MyVirtualDisk::GetInstance();

	for (auto &_dir : targetdir->GetDirChildren())
	{
		this->DeleteDirRecursion(_dir.second);
	}
	if (targetdir == virtual_disk->GetCurrentDir())
	{
		std::cout << "另一个程序正在使用此文件，进程无法访问。" << std::endl;
		std::cout << std::endl;
		return;
	}
	auto parent_dir = targetdir->GetParentDir();
	parent_dir->DeleteChild(targetdir->GetName());
	delete targetdir;
}

void MyDir::DeleteDirRecursionForQuit(MyDir * targetdir)
{
	auto virtual_disk = MyVirtualDisk::GetInstance();

	for (auto &_dir : targetdir->GetDirChildren())
	{
		this->DeleteDirRecursion(_dir.second);
	}
	if (targetdir == virtual_disk->GetCurrentDir())
	{
		delete targetdir;
		return;
	}
	auto parent_dir = targetdir->GetParentDir();
	parent_dir->DeleteChild(targetdir->GetName());
	delete targetdir;
}

void MyDir::AddChild(MyFileBase * new_file)
{
	auto name_upper = boost::to_upper_copy(new_file->GetName());
	this->m_children[name_upper] = new_file;
}

void MyDir::AddChild(std::string name, MyFileBase * new_file)
{
	boost::to_upper(name);
	this->m_children[name] = new_file;
	this->SetLastModifiedTime();
}

void MyDir::AddChild(std::string name, MyFileBase * new_file, bool is_override)
{

}

void MyDir::MyDelete(bool is_force)
{
	this->GetParentDir()->DeleteChild(this->GetName());
	this->SetParentDir(nullptr);
	delete this;
}

void MyDir::DeleteChild(std::string name)
{
	// 把子文件从map中移除，并没有清理对象
	boost::to_upper(name);
	this->m_children.erase(this->m_children.find(name));
	this->SetLastModifiedTime();
}

MyFileBase* MyDir::FindChild(std::string file_name)
{
	auto children_map = this->GetChildren();
	auto new_name_upper = boost::to_upper_copy(file_name);
	auto child_itr = children_map.find(new_name_upper);
	// 找到了
	if (child_itr == children_map.cend())
	{
		return nullptr;
	}
	return child_itr->second;
}

