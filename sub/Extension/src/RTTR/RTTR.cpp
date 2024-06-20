#include <assert.h>

#include <algorithm>

#include "RTTR.hpp"

namespace RTTR
{
	class TypeInfoImpl
	{
	public:
		static inline std::unordered_map<std::string, TypeInfo*> s_types;

	public:
		TypeInfoImpl() = default;

	public:
		std::unordered_map<std::string, SuperclassInfo> superclass;			//����

		std::unordered_map<std::string, StaticMemberInfo> staticMembers;	//��̬��Ա����
		std::unordered_map<std::string, NormalMemberInfo> normalMembers;	//��ͨ��Ա����

		std::unordered_multimap<std::string, StaticMethodInfo> staticMethods;	//��̬����
		std::unordered_multimap<std::string, NormalMethodInfo> normalMethods;	//��ͨ����
	};
}

RTTR::TypeInfo* RTTR::TypeInfo::find(const std::string& name)
{
	auto find{ TypeInfoImpl::s_types.find(name) };
	return find != TypeInfoImpl::s_types.end() ? find->second : nullptr;
}

bool RTTR::TypeInfo::registerType(TypeInfo* type)
{
	assert(type != nullptr);

	if (find(type->name()) == nullptr)
	{
		TypeInfoImpl::s_types.insert({ type->name(), type });
		return true;
	}
	return false;
}

RTTR::TypeInfo::TypeInfo() :
	m_impl(new TypeInfoImpl{})
{
}

RTTR::TypeInfo::~TypeInfo() = default;

void RTTR::TypeInfo::registerSuperclass(const SuperclassInfo& info)
{
	assert(info.interview != RTTR::None && info.info != nullptr);

	if (false == superclass(info.info->name()).has_value()) m_impl->superclass.insert({ info.info->name(), info });
}

std::list<std::string> RTTR::TypeInfo::superclassNames() const
{
	std::list<std::string> names;
	for (const auto& [name, info] : m_impl->superclass) names.push_back(name);
	return names;
}

std::optional<RTTR::SuperclassInfo> RTTR::TypeInfo::superclass(const std::string& name) const
{
	auto find{ m_impl->superclass.find(name) };
	return find != m_impl->superclass.end() ? std::optional{ find->second } : std::nullopt;
}

bool RTTR::TypeInfo::registerStaticMember(const StaticMemberInfo& info)
{
	assert(false == info.name.empty() && info.interview != None && info.info != nullptr && info.address != nullptr);

	if (false == staticMember(info.name).has_value())
	{
		m_impl->staticMembers.insert({ info.name, info });
		return true;
	}
	return false;
}

std::list<std::string> RTTR::TypeInfo::staticMemberNames() const
{
	std::list<std::string> names;
	for (const auto& [name, info] : m_impl->staticMembers) names.push_back(name);
	return names;
}

std::optional<RTTR::StaticMemberInfo> RTTR::TypeInfo::staticMember(const std::string& name) const
{
	auto find{ m_impl->staticMembers.find(name) };
	return find != m_impl->staticMembers.end() ? std::optional{ find->second } : std::nullopt;
}

bool RTTR::TypeInfo::registerNormalMember(const NormalMemberInfo& info)
{
	assert(false == info.name.empty() && info.interview != None && info.info != nullptr && info.offset >= 0);

	if (false == normalMember(info.name).has_value())
	{
		m_impl->normalMembers.insert({ info.name, info });
		return true;
	}
	return false;
}

std::list<std::string> RTTR::TypeInfo::normalMemberNames() const
{
	std::list<std::string> names;
	for (const auto& [name, info] : m_impl->normalMembers) names.push_back(name);
	return names;
}

std::optional<RTTR::NormalMemberInfo> RTTR::TypeInfo::normalMember(const std::string& name) const
{
	auto find{ m_impl->normalMembers.find(name) };
	return find != m_impl->normalMembers.end() ? std::optional{ find->second } : std::nullopt;
}

bool RTTR::TypeInfo::registerStaticMethod(const StaticMethodInfo& info)
{
	assert(false == info.name.empty() && info.interview != None && info.returnInfo != nullptr && info.address != nullptr);

	if (auto methods{ staticMethod(info.name) }; std::find_if(methods.begin(), methods.end(), [&info](const StaticMethodInfo& val) { return val.address == info.address; }) == methods.end())
	{
		m_impl->staticMethods.insert({ info.name, info });
		return true;
	}
	return false;
}

std::unordered_set<std::string> RTTR::TypeInfo::staticMethodNames() const
{
	std::unordered_set<std::string> names;
	for (const auto& [name, info] : m_impl->staticMethods) names.insert(name);
	return names;
}

std::list<RTTR::StaticMethodInfo> RTTR::TypeInfo::staticMethod(const std::string& name) const
{
	std::list<RTTR::StaticMethodInfo> methods;
	auto pair{ m_impl->staticMethods.equal_range(name) };
	while (pair.first != pair.second) methods.push_back(pair.first++->second);
	return methods;
}
