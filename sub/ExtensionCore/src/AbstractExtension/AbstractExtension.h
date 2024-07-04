#pragma once

#include <string>
#include <memory>

#include "AbstractCoreExtension.h"

#include "ExtensionCore_global.h"

class AbstractLogic;

class AbstractExtensionImpl;
class EXTENSIONCORE_API AbstractExtension : public AbstractCoreExtension
{
public:
	AbstractExtension();
	~AbstractExtension();

protected:
	/// <summary>
	/// ����logic
	/// </summary>
	/// <returns></returns>
	virtual AbstractLogic* createLogic() = 0;

public:
	/// <summary>
	/// ��ȡlogic
	/// </summary>
	/// <returns></returns>
	AbstractLogic* logic();

protected:
	std::unique_ptr<AbstractExtensionImpl> m_impl{};
};

#define EXPORT_EXTENSION_FUNC_NAME ("exportExtension")
using ExportExtensionFunc = AbstractExtension* (*)();
extern "C" { __declspec(dllexport) extern AbstractExtension* exportExtension(); }
