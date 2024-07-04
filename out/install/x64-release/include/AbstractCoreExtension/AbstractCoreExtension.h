#pragma once

#include <string>

#include "ExtensionCore_global.h"

class EXTENSIONCORE_API AbstractCoreExtension
{
public:
	AbstractCoreExtension() = default;
	virtual ~AbstractCoreExtension() = default;

public:
	virtual std::string name() const = 0;
};