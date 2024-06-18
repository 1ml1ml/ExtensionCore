#pragma once

#include <string>
#include <memory>
#include <filesystem>
#include <unordered_map>

class AbstractExtension;

class ExtensionManagerImpl;
class ExtensionManager
{
public:
	/// <summary>
	/// ��ȡ��ʵ��
	/// </summary>
	/// <returns></returns>
	static ExtensionManager* instance();

public:
	/// <summary>
	/// ������չĿ¼
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	std::unordered_map<std::string, AbstractExtension*> loadExtensionDir(const std::filesystem::path& path);

	/// <summary>
	/// ������չ�ļ�
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	AbstractExtension* loadExtensionFile(const std::filesystem::path& path);

	/// <summary>
	/// ��ȡ��չ
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	AbstractExtension* extension(const std::string& name) const;

	/// <summary>
	/// ��ȡ������չ
	/// </summary>
	/// <returns></returns>
	std::unordered_map<std::string, AbstractExtension*> extensions() const;

private:
	/// <summary>
	/// ���һ����չ
	/// </summary>
	/// <param name="extension"></param>
	/// <returns></returns>
	bool addExtension(AbstractExtension* extension);

private:
	ExtensionManager() = default;
	~ExtensionManager() = default;

private:
	std::unique_ptr<ExtensionManagerImpl> m_impl;
};