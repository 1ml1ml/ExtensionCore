#include <assert.h>

#include <algorithm>

#include "RTTR.hpp"

namespace RTTR
{
	class TypeImpl
	{
	public:
		static inline std::unordered_map<std::string, Type*> s_types;

	public:
		TypeImpl() = default;

	public:
		std::unordered_map<std::string, SuperclassInfo> superclass;			//父类

		std::unordered_map<std::string, StaticMemberInfo> staticMembers;	//静态成员变量
		std::unordered_map<std::string, NormalMemberInfo> normalMembers;	//普通成员变量
	};
}

RTTR::Type* RTTR::Type::find(const std::string& name)
{
	auto find{ TypeImpl::s_types.find(name) };
	return find != TypeImpl::s_types.end() ? find->second : nullptr;
}

void RTTR::Type::registerType(Type* type)
{
	assert(type != nullptr);

	if (find(type->name()) == nullptr) TypeImpl::s_types.insert({ type->name(), type });
}

RTTR::Type::Type() :
	m_impl(new TypeImpl{})
{
}

RTTR::Type::~Type() = default;

void RTTR::Type::registerSuperclass(const SuperclassInfo& info)
{
	assert(info.interview != RTTR::None && info.type != nullptr);

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

void RTTR::Type::registerStaticMember(const StaticMemberInfo& info)
{
	assert(false == info.name.empty() && info.interview != None && info.type != nullptr && info.address != nullptr);

	if (false == staticMember(info.name).has_value()) m_impl->staticMembers.insert({ info.name, info });
}

std::list<std::string> RTTR::Type::staticMemberNames() const
{
	std::list<std::string> names;
	for (const auto& [name, info] : m_impl->staticMembers) names.push_back(name);
	return names;
}

std::optional<RTTR::StaticMemberInfo> RTTR::Type::staticMember(const std::string& name) const
{
	auto find{ m_impl->staticMembers.find(name) };
	return find != m_impl->staticMembers.end() ? std::optional{ find->second } : std::nullopt;
}

void RTTR::Type::registerNormalMember(const NormalMemberInfo& info)
{
	assert(false == info.name.empty() && info.interview != None && info.type != nullptr && info.offset >= 0);

	if (false == normalMember(info.name).has_value()) m_impl->normalMembers.insert({ info.name, info });
}

std::list<std::string> RTTR::Type::normalMemberNames() const
{
	std::list<std::string> names;
	for (const auto& [name, info] : m_impl->normalMembers) names.push_back(name);
	return names;
}

std::optional<RTTR::NormalMemberInfo> RTTR::Type::normalMember(const std::string& name) const
{
	auto find{ m_impl->normalMembers.find(name) };
	return find != m_impl->normalMembers.end() ? std::optional{ find->second } : std::nullopt;
}
