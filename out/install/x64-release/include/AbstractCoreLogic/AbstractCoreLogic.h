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
	/// ��ȡlogic����չ����
	/// </summary>
	/// <returns></returns>
	AbstractExtension* extension() const;

private:
	std::unique_ptr<AbstractCoreLogiImpl> m_impl{};
};