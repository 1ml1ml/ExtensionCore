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
	/// 获取单实例
	/// </summary>
	/// <returns></returns>
	static ExtensionManager* instance();

public:
	/// <summary>
	/// 加载扩展目录
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	std::unordered_map<std::string, AbstractExtension*> loadExtensionDir(const std::filesystem::path& path);

	/// <summary>
	/// 加载扩展文件
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	AbstractExtension* loadExtensionFile(const std::filesystem::path& path);

	/// <summary>
	/// 获取扩展
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	AbstractExtension* extension(const std::string& name) const;

	/// <summary>
	/// 获取所有扩展
	/// </summary>
	/// <returns></returns>
	std::unordered_map<std::string, AbstractExtension*> extensions() const;

private:
	/// <summary>
	/// 添加一个扩展
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