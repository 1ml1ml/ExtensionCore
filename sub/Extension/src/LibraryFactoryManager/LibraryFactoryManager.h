#pragma once

#include <string>
#include <memory>
#include <filesystem>
#include <unordered_map>

#include "LibraryFactory.h"

class LibraryFactoryManagerImpl;
class LibraryFactoryManager
{
public:
	/// <summary>
	/// 获取单实例
	/// </summary>
	/// <returns></returns>
	static LibraryFactoryManager* instance();

public:
	/// <summary>
	/// 加载工厂目录
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	std::unordered_map<std::filesystem::path, LibraryFactory*> loadFactoryDir(const std::filesystem::path& path);

	/// <summary>
	/// 加载工厂文件
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	LibraryFactory* loadFactoryFile(const std::filesystem::path& path);

	/// <summary>
	/// 获取工厂
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	LibraryFactory* factory(const std::string& name) const;

	/// <summary>
	/// 获取所有工厂
	/// </summary>
	/// <returns></returns>
	std::unordered_map<std::filesystem::path, LibraryFactory*> factorys() const;

private:
	/// <summary>
	/// 添加一个工厂
	/// </summary>
	/// <param name="factory"></param>
	/// <returns></returns>
	bool addFactory(LibraryFactory* factory);

private:
	LibraryFactoryManager() = default;
	~LibraryFactoryManager() = default;

private:
	std::unique_ptr<LibraryFactoryManagerImpl> m_impl{};
};