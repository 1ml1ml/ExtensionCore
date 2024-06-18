#pragma once

#include <memory>

class AbstractExtension;

class AbstractLogiImpl;
class AbstractLogic
{
public:
	AbstractLogic(AbstractExtension* extension);

public:
	/// <summary>
	/// ��ȡlogic����չ����
	/// </summary>
	/// <returns></returns>
	AbstractExtension* extension() const;

private:
	std::unique_ptr<AbstractLogiImpl> m_impl{};
};
