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
	/// �����Ƿ���Ч
	/// </summary>
	/// <returns></returns>
	bool valid() const;

	/// <summary>
	/// ��ȡ��������Ŀ�·��
	/// </summary>
	/// <returns></returns>
	std::filesystem::path path() const;

	/// <summary>
	/// ͨ����������ȡ������ַ
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	void* func(const std::string& name) const;

private:
	std::unique_ptr<LibraryFactoryImpl> m_impl{};
};