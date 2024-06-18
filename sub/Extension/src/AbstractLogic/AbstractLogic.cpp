#include "AbstractLogic.h"

class AbstractLogiImpl
{
public:
	AbstractLogiImpl() = default;

public:
	AbstractExtension* extension{ nullptr };
};

AbstractLogic::AbstractLogic(AbstractExtension* extension) : 
	m_impl(new AbstractLogiImpl{})
{
	m_impl->extension = extension;
}

AbstractExtension* AbstractLogic::extension() const
{
	return m_impl->extension;
}
