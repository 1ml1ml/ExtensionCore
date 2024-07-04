#pragma once

#include <string>
#include <memory>
#include <filesystem>

#include "LibraryCoreFactory.h"

#include "ExtensionCore_global.h"

class AbstractExtension;

class LibraryFactoryImpl;
class EXTENSIONCORE_API LibraryFactory : public LibraryCoreFactory
{
public:
	LibraryFactory(const std::filesystem::path& path);
	~LibraryFactory();

public:
	/// <summary>
	/// 工厂是否有效
	/// </summary>
	/// <returns></returns>
	bool valid() const;

	/// <summary>
	/// 获取工厂管理的库路径
	/// </summary>
	/// <returns></returns>
	std::filesystem::path path() const;

	/// <summary>
	/// 通过函数名获取函数地址
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	void* func(const std::string& name) const;

private:
	std::unique_ptr<LibraryFactoryImpl> m_impl{};
};