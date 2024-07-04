#include "AbstractCoreLogic.h"

class AbstractCoreLogiImpl
{
public:
	AbstractCoreLogiImpl() = default;

public:
	AbstractExtension* extension{ nullptr };
};

AbstractCoreLogic::AbstractCoreLogic(AbstractExtension* extension) :
	m_impl(new AbstractCoreLogiImpl{})
{
	m_impl->extension = extension;
}

AbstractCoreLogic::~AbstractCoreLogic() = default;

AbstractExtension* AbstractCoreLogic::extension() const
{
	return m_impl->extension;
}