#include "AbstractExtension.h"

class AbstractExtensionImpl
{
public:
	AbstractExtensionImpl() = default;

public:
	AbstractLogic* logic{ nullptr };
};

AbstractExtension::AbstractExtension() : AbstractCoreExtension(),
m_impl(new AbstractExtensionImpl{})
{
}

AbstractExtension::~AbstractExtension() = default;

AbstractLogic* AbstractExtension::logic()
{
	if (nullptr == m_impl->logic) m_impl->logic = createLogic();
	return m_impl->logic;
}

