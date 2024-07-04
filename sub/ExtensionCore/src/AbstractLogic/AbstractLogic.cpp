#include "AbstractLogic.h"

class AbstractLogiImpl
{
public:
	AbstractLogiImpl() = default;
};

AbstractLogic::AbstractLogic(AbstractExtension* extension) : AbstractCoreLogic(extension), 
m_impl(new AbstractLogiImpl{})
{
}
