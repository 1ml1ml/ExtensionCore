#pragma once

#include <list>
#include <string>
#include <memory>
#include <optional>
#include <unordered_map>

namespace RTTR
{
	class Type;

	enum Interview : unsigned char
	{
		None = 0,
		Public,
		Protected,
		Private,
	};

	struct SuperclassInfo
	{
		Interview interview{ None };
		Type* type{ nullptr };
	};

	struct MemberInfo
	{
		std::string name{};
		Interview interview{ None };
		Type* type{ nullptr };
	};

	struct StaticMemberInfo : public MemberInfo
	{
		void* address{ nullptr };
	};

	struct NormalMemberInfo : public MemberInfo
	{
		int* offset{ nullptr };
	};

	struct MethodInfo
	{
		std::string name{};
		Interview interview{ None };
		Type* returnType{ nullptr };
		std::list<std::pair<std::string, Type*>> args{};
		void* address{ nullptr };
	};

	struct StaticMethodInfo : public MethodInfo
	{

	};

	struct NormalMethodInfo : public MethodInfo
	{

	};

	class TypeImpl;
	class Type
	{
	public:
		/// <summary>
		/// ����һ������
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		static Type* find(const std::string& name);

	protected:
		/// <summary>
		/// ע��һ������
		/// </summary>
		/// <param name="type"></param>
		static void registerType(Type* type);

	public:
		Type();
		virtual ~Type();

	public:
		/// <summary>
		/// ��ȡ��������
		/// </summary>
		/// <returns></returns>
		virtual std::string name() const = 0;

		/// <summary>
		/// ��ȡ���ʹ�С
		/// </summary>
		/// <returns></returns>
		virtual size_t size() const = 0;

	public:
		/// <summary>
		/// ע�Ḹ����Ϣ
		/// </summary>
		/// <param name="info"></param>
		void registerSuperclass(const SuperclassInfo& info);

		/// <summary>
		/// ��ȡ��������
		/// </summary>
		/// <returns></returns>
		std::list<std::string> superclassNames() const;

		/// <summary>
		/// ��ȡ������Ϣ
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		std::optional<SuperclassInfo> superclass(const std::string& name) const;

		/// <summary>
		/// ע�ᾲ̬��Ա������Ϣ
		/// </summary>
		/// <param name="info"></param>
		void registerStaticMember(const StaticMemberInfo& info);

		/// <summary>
		/// ��ȡ���о�̬��Ա������
		/// </summary>
		/// <returns></returns>
		std::list<std::string> staticMemberNames() const;

		/// <summary>
		/// ��ȡ��̬��Ա������Ϣ
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		std::optional<StaticMemberInfo> staticMember(const std::string& name) const;

		/// <summary>
		/// ע����ͨ��Ա������Ϣ
		/// </summary>
		/// <param name="info"></param>
		void registerNormalMember(const NormalMemberInfo& info);

		/// <summary>
		/// ��ȡ������ͨ��Ա������
		/// </summary>
		/// <returns></returns>
		std::list<std::string> normalMemberNames() const;

		/// <summary>
		/// ��ȡ��ͨ��Ա������Ϣ
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		std::optional<NormalMemberInfo> normalMember(const std::string& name) const;

	private:
		std::unique_ptr<TypeImpl> m_impl{};
	};

	template<typename T>
	class RealType : public Type { };
}

//ע������
#define RTTR_REGISTER_TYPE(T) \
namespace RTTR \
{ \
	template<> \
	class RealType<T> final : public Type \
	{ \
	public: \
		using Value = T; \
\
	public: \
		static RealType<T>* instance() \
		{ \
			static RealType<T> instance{}; \
			return &instance; \
		} \
\
	public: \
		std::string name() const override { return #T; } \
		size_t size() const override { return sizeof(T); } \
\
	private: \
		RealType<T>() : Type() { registerType(this); } \
		~RealType<T>() = default; \
	}; \
}

//ע�Ḹ��
#define RRTR_REGISTER_SUPERCLASS(Interview, Superclass) \
do \
{ \
	using T = std::remove_reference<decltype(*this)>::type; \
	RTTR::RealType<T>::instance()->registerSuperclass({Interview, RTTR::RealType<Superclass>::instance()}); \
} \
while (false)

//ע�ᾲ̬��Ա����
#define RTTR_REGISTER_STATIC_MEMBER(Interview, Name) \
do \
{ \
	using T = std::remove_reference<decltype(*this)>::type; \
	RTTR::RealType<T>::instance()->registerStaticMember({#Name, Interview, RTTR::RealType<decltype(T::Name)>::instance(), &T::Name}); \
} \
while (false)

//ע������ͨ��Ա����
#define RTTR_REGISTER_NORMAL_MEMBER(Interview, Name) \
do \
{ \
	using T = std::remove_reference<decltype(*this)>::type; \
	auto offset{ &T::Name }; \
	RTTR::RealType<T>::instance()->registerNormalMember({#Name, Interview, RTTR::RealType<decltype(T::Name)>::instance(), (int*)(&offset)}); \
} \
while (false)
