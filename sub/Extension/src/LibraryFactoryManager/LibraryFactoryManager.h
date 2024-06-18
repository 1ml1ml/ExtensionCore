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
	/// ��ȡ��ʵ��
	/// </summary>
	/// <returns></returns>
	static LibraryFactoryManager* instance();

public:
	/// <summary>
	/// ���ع���Ŀ¼
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	std::unordered_map<std::filesystem::path, LibraryFactory*> loadFactoryDir(const std::filesystem::path& path);

	/// <summary>
	/// ���ع����ļ�
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	LibraryFactory* loadFactoryFile(const std::filesystem::path& path);

	/// <summary>
	/// ��ȡ����
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	LibraryFactory* factory(const std::string& name) const;

	/// <summary>
	/// ��ȡ���й���
	/// </summary>
	/// <returns></returns>
	std::unordered_map<std::filesystem::path, LibraryFactory*> factorys() const;

private:
	/// <summary>
	/// ���һ������
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