#include <assert.h>

#include <algorithm>

#include "RTTR.hpp"

namespace RTTR
{
	class TypeImpl
	{
	public:
		TypeImpl() = default;

	public:
		std::unordered_map<std::string, SuperclassInfo> superclass;	//父类
		std::unordered_map<std::string, NormalMemberInfo> members;		//成员变量
	};
}

RTTR::Type* RTTR::Type::find(const std::string& name)
{
	auto find{ s_types.find(name) };
	return find != s_types.end() ? find->second : nullptr;
}

RTTR::Type::Type() :
	m_impl(new TypeImpl{})
{
}

RTTR::Type::~Type() = default;

void RTTR::Type::registerSuperclass(const SuperclassInfo& info)
{
	assert(info.im != RTTR::None && info.type != nullptr);

	if (false == superclass(info.type->name()).has_value()) m_impl->superclass.insert({ info.type->name(), info });
}

std::list<std::string> RTTR::Type::superclassNames() const
{
	std::list<std::string> names;
	for (const auto& [name, info] : m_impl->superclass) names.push_back(name);
	return names;
}

std::optional<RTTR::SuperclassInfo> RTTR::Type::superclass(const std::string& name) const
{
	auto find{ m_impl->superclass.find(name) };
	return find != m_impl->superclass.end() ? std::optional{ find->second } : std::nullopt;
}

void RTTR::Type::registerNormalMember(const NormalMemberInfo& info)
{
	assert(false == info.name.empty() && info.im != None && info.type != nullptr && info.offset >= 0);

	if (false == normalMember(info.name).has_value()) m_impl->members.insert({ info.name, info });
}

std::list<std::string> RTTR::Type::normalMemberNames() const
{
	std::list<std::string> names;
	for (const auto& [name, info] : m_impl->members) names.push_back(name);
	return names;
}

std::optional<RTTR::NormalMemberInfo> RTTR::Type::normalMember(const std::string& name) const
{
	auto find{ m_impl->members.find(name) };
	return find != m_impl->members.end() ? std::optional{ find->second } : std::nullopt;
}

