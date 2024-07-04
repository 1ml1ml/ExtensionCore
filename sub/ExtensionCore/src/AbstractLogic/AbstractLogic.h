#pragma once

#include <memory>

#include "AbstractCoreLogic.h"

#include "ExtensionCore_global.h"

class AbstractLogiImpl;
class EXTENSIONCORE_API AbstractLogic : public AbstractCoreLogic
{
public:
	AbstractLogic(AbstractExtension* extension);

private:
	std::unique_ptr<AbstractLogiImpl> m_impl{};
};
