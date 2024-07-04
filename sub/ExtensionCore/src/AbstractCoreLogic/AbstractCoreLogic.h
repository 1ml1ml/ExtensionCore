#pragma once

#include <memory>

#include "ExtensionCore_global.h"

class AbstractExtension;

class AbstractCoreLogiImpl;
class EXTENSIONCORE_API AbstractCoreLogic
{
public:
	AbstractCoreLogic(AbstractExtension* extension);
	~AbstractCoreLogic();

public:
	/// <summary>
	/// 获取logic的扩展对象
	/// </summary>
	/// <returns></returns>
	AbstractExtension* extension() const;

private:
	std::unique_ptr<AbstractCoreLogiImpl> m_impl{};
};