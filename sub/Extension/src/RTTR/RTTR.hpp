#pragma once

#include <list>
#include <string>
#include <memory>
#include <optional>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

namespace RTTR
{
	class TypeInfo;

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
		TypeInfo* info{ nullptr };
	};

	struct MemberInfo
	{
		std::string name{};
		Interview interview{ None };
		TypeInfo* info{ nullptr };
	};

	struct StaticMemberInfo : public MemberInfo
	{
		void* address{ nullptr };
	};

	struct NormalMemberInfo : public MemberInfo
	{
		int offset{ 0 };
	};

	struct MethodInfo
	{
	public:
		template<typename... Args>
		static std::list<TypeInfo*> unpackArgs() 
		{
			std::list<TypeInfo*> args;
			(args.push_back(RealTypeInfo<Args>::instance()), ...);
			return args;
		}

	public:
		std::string name{};
		Interview interview{ None };
		TypeInfo* returnInfo{ nullptr };
		std::list<TypeInfo*> args{};
		void* address{ nullptr };
	};

	struct StaticMethodInfo : public MethodInfo { };

	struct NormalMethodInfo : public MethodInfo { };

	class TypeInfoImpl;
	class TypeInfo
	{
	public:
		/// <summary>
		/// ����һ������
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		static TypeInfo* find(const std::string& name);

		template<typename T>
		static size_t Sizeof()
		{
			if constexpr (std::is_void_v<T>) return 0;
			else return sizeof(T);
		}

	protected:
		/// <summary>
		/// ע��һ������
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		static bool registerType(TypeInfo* type);

	public:
		TypeInfo();
		virtual ~TypeInfo();

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
		/// <returns></returns>
		bool registerStaticMember(const StaticMemberInfo& info);

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
		/// <returns></returns>
		bool registerNormalMember(const NormalMemberInfo& info);

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

		/// <summary>
		/// ע�ᾲ̬����
		/// </summary>
		/// <param name="info"></param>
		/// <returns></returns>
		bool registerStaticMethod(const StaticMethodInfo& info);

		/// <summary>
		/// ��ȡ���о�̬��������
		/// </summary>
		/// <returns></returns>
		std::unordered_set<std::string> staticMethodNames() const;

		/// <summary>
		/// ��ȡ��̬������Ϣ
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		std::list<StaticMethodInfo> staticMethod(const std::string& name) const;

	private:
		std::unique_ptr<TypeInfoImpl> m_impl{};
	};

	template<typename T>
	class RealTypeInfo : public TypeInfo { };
}

//ע������
#define RTTR_REGISTER_TYPE(T) \
namespace RTTR \
{ \
	template<> \
	class RealTypeInfo<T> final : public TypeInfo \
	{ \
	public: \
		using Value = T; \
\
	public: \
		static RealTypeInfo<T>* instance() \
		{ \
			static RealTypeInfo<T> instance{}; \
			return &instance; \
		} \
\
	public: \
		std::string name() const override { return #T; } \
		size_t size() const override { return Sizeof<T>(); } \
\
	private: \
		RealTypeInfo<T>() : TypeInfo() { registerType(this); } \
		~RealTypeInfo<T>() = default; \
	}; \
}

//ע�Ḹ��
#define RRTR_REGISTER_SUPERCLASS(Interview, Superclass) \
do \
{ \
	using T = std::remove_reference_t<decltype(*this)>; \
	RTTR::RealTypeInfo<T>::instance()->registerSuperclass({ Interview, RTTR::RealTypeInfo<Superclass>::instance() }); \
} \
while (false)

//ע�ᾲ̬��Ա����
#define RTTR_REGISTER_STATIC_MEMBER(T, Interview, Name) \
do \
{ \
	RTTR::RealTypeInfo<T>::instance()->registerStaticMember({ #Name, Interview, RTTR::RealTypeInfo<decltype(T::Name)>::instance(), &T::Name }); \
} \
while (false)

//ע������ͨ��Ա����
#define RTTR_REGISTER_NORMAL_MEMBER(Interview, Name) \
do \
{ \
	using T = std::remove_reference_t<decltype(*this)>; \
	auto offset{ &T::Name }; \
	RTTR::RealTypeInfo<T>::instance()->registerNormalMember({ #Name, Interview, RTTR::RealTypeInfo<decltype(T::Name)>::instance(), *(int*)(&offset) }); \
} \
while (false)

//ע�ᾲ̬����
#define RTTR_REGISTER_STATIC_METHOD(T, Interview, Name, ...) \
do \
{ \
	RTTR::RealTypeInfo<T>::instance()->registerStaticMethod({ #Name, Interview, RTTR::RealTypeInfo<std::invoke_result_t<decltype(T::Name), __VA_ARGS__>>::instance(), RTTR::MethodInfo::unpackArgs<__VA_ARGS__>(), T::Name }); \
} \
while (false)
