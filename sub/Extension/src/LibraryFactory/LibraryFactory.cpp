#include <Windows.h>

#include <list>

#include "LibraryFactory.h"
#include "AbstractExtension.h"

class LibraryFactoryImpl
{
public:
	LibraryFactoryImpl() = default;

public:
	std::filesystem::path path{};

	HMODULE handle{ 0 };
};

LibraryFactory::LibraryFactory(const std::filesystem::path& path) : LibraryCoreFactory(),
m_impl(new LibraryFactoryImpl{})
{
	m_impl->path = std::filesystem::absolute(path);
	m_impl->handle = ::LoadLibrary(m_impl->path.string().c_str());
}

LibraryFactory::~LibraryFactory()
{
	if (valid())
	{
		::FreeLibrary(m_impl->handle);
		m_impl->handle = 0;
	}
}

bool LibraryFactory::valid() const
{
	return m_impl->handle;
}

std::filesystem::path LibraryFactory::path() const
{
	return m_impl->path;
}

void* LibraryFactory::func(const std::string& name) const
{
	return ::GetProcAddress(m_impl->handle, name.c_str());
}

