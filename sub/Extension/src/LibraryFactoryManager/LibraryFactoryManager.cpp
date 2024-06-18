#include <algorithm>

#include "LibraryFactoryManager.h"

class LibraryFactoryManagerImpl
{
public:
	LibraryFactoryManagerImpl() = default;

public:
	std::unordered_map<std::filesystem::path, std::unique_ptr<LibraryFactory>> factorys{};
};

LibraryFactoryManager* LibraryFactoryManager::instance()
{
	static LibraryFactoryManager instance{};
	return &instance;
}

std::unordered_map<std::filesystem::path, LibraryFactory*> LibraryFactoryManager::loadFactoryDir(const std::filesystem::path& path)
{
	std::unordered_map<std::filesystem::path, LibraryFactory*> factorys;
	for (const auto& entry : std::filesystem::directory_iterator{ path })
	{
		if (entry.is_directory())
		{
			auto dirFactorys{ loadFactoryDir(entry.path()) };
			factorys.insert(dirFactorys.begin(), dirFactorys.end());
		}
		else if (entry.is_regular_file())
		{
			if (auto factory{ loadFactoryFile(entry.path()) }; factory != nullptr)
				factorys.insert({ factory->path(), factory });
		}
	}
	return factorys;
}

LibraryFactory* LibraryFactoryManager::loadFactoryFile(const std::filesystem::path& path)
{
	if (std::filesystem::exists(path))
	{
		if (auto factory{ new LibraryFactory{path} }; addFactory(factory)) return factory;
		else delete factory;
	}
	return nullptr;
}

LibraryFactory* LibraryFactoryManager::factory(const std::string& name) const
{
	auto find{ m_impl->factorys.find(name) };
	return find != m_impl->factorys.end() ? find->second.get() : nullptr;
}

std::unordered_map<std::filesystem::path, LibraryFactory*> LibraryFactoryManager::factorys() const
{
	std::unordered_map<std::filesystem::path, LibraryFactory*> factorys;
	for (const auto& [path, factory] : m_impl->factorys) factorys.insert({ path, factory.get() });
	return factorys;
}

bool LibraryFactoryManager::addFactory(LibraryFactory* factory)
{
	if (factory != nullptr && factory->valid() && 0 == m_impl->factorys.count(factory->path()))
	{
		m_impl->factorys.insert({ factory->path(), std::unique_ptr<LibraryFactory>{factory}});
		return true;
	}
	return false;
}

