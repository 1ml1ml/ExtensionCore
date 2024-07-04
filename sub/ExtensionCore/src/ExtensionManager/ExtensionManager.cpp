#include <memory>

#include "ExtensionManager.h"
#include "AbstractExtension.h"
#include "LibraryFactoryManager.h"

class ExtensionManagerImpl
{
public:
	ExtensionManagerImpl() = default;

public:
	std::unordered_map<std::string, std::unique_ptr<AbstractExtension>> extensions{};
};

ExtensionManager* ExtensionManager::instance()
{
	static ExtensionManager instance{};
	return &instance;
}

std::unordered_map<std::string, AbstractExtension*> ExtensionManager::loadExtensionDir(const std::filesystem::path& path)
{
	std::unordered_map<std::string, AbstractExtension*> extensions;
	for (const auto& [path, factory] : LibraryFactoryManager::instance()->loadFactoryDir(path))
	{
		if (auto func{ reinterpret_cast<ExportExtensionFunc>(factory->func(EXPORT_EXTENSION_FUNC_NAME)) }; func != nullptr)
		{
			if (auto extension{ func() }; addExtension(extension))
				extensions.insert({ extension->name(), extension });
		}
	}
	return extensions;
}

AbstractExtension* ExtensionManager::loadExtensionFile(const std::filesystem::path& path)
{
	if (auto factory{ LibraryFactoryManager::instance()->loadFactoryFile(path) }; factory != nullptr)
	{
		if (auto func{ reinterpret_cast<ExportExtensionFunc>(factory->func(EXPORT_EXTENSION_FUNC_NAME)) }; func != nullptr)
		{
			if (auto extension{ func() }; addExtension(extension))
				return extension;
		}
	}
	return nullptr;
}

AbstractExtension* ExtensionManager::extension(const std::string& name) const
{
	auto find{ m_impl->extensions.find(name) };
	return find != m_impl->extensions.end() ? find->second.get() : nullptr;
}

std::unordered_map<std::string, AbstractExtension*> ExtensionManager::extensions() const
{
	std::unordered_map<std::string, AbstractExtension*> extensions;
	for (const auto& [name, extension] : m_impl->extensions) extensions.insert({ name, extension.get() });
	return extensions;
}

bool ExtensionManager::addExtension(AbstractExtension* extension)
{
	if (extension != nullptr && 0 == m_impl->extensions.count(extension->name()))
	{
		m_impl->extensions.insert({ extension->name(), std::unique_ptr<AbstractExtension>(extension) });
		return true;
	}
	return false;
}
