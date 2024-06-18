#pragma once

#include <string>

class AbstractCoreExtension
{
public:
	AbstractCoreExtension() = default;

public:
	virtual std::string name() const = 0;
};