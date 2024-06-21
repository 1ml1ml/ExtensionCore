#pragma once

#include <list>
#include <string>
#include <memory>
#include <optional>
#include <typeinfo>
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

	/// <summary>
	/// ������Ϣ
	/// </summary>
	struct SuperclassInfo
	{
		Interview interview{ None };
		TypeInfo* info{ nullptr };
	};

	/// <summary>
	/// ��Ա��Ϣ
	/// </summary>
	struct MemberInfo
	{
	public:
		std::string name{};
		Interview interview{ None };
		TypeInfo* info{ nullptr };
	};

	/// <summary>
	/// ��̬��Ա��Ϣ
	/// </summary>
	struct StaticMemberInfo : public MemberInfo
	{
	public:
		/// <summary>
		/// ��ȡ��̬��Ա��ֵ
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<typename T>
		T value() { return *reinterpret_cast<T*>(address); }

	public:
		void* address{ nullptr };
	};

	/// <summary>
	/// ��ͨ��Ա��Ϣ
	/// </summary>
	struct NormalMemberInfo : public MemberInfo
	{
	public:
		/// <summary>
		/// ��ȡ��ͨ��Ա��ֵ
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <typeparam name="M"></typeparam>
		/// <param name="o"></param>
		/// <returns></returns>
		template<typename T, typename M>
		M value(T* o) { return *reinterpret_cast<M*>((reinterpret_cast<unsigned char*>(o) + offset)); }

	public:
		int offset{ 0 };
	};

	/// <summary>
	/// ������Ϣ
	/// </summary>
	struct MethodInfo
	{
	public:
		/// <summary>
		/// ���÷���
		/// </summary>
		/// <typeparam name="F">��������</typeparam>
		/// <typeparam name="...Args"></typeparam>
		/// <param name="...args"></param>
		/// <returns></returns>
		template<typename F, typename... Args>
		decltype(auto) invoke(Args&&... args) { return std::invoke(*(F*)&address, std::forward<Args>(args)...); }

	public:
		std::string name{};
		Interview interview{ None };
		TypeInfo* returnInfo{ nullptr };
		std::list<TypeInfo*> args{};
		void* address{ nullptr };
	};

	/// <summary>
	/// ��̬������Ϣ
	/// </summary>
	struct StaticMethodInfo : public MethodInfo { };

	struct NormalMethodInfo : public MethodInfo { };

	class TypeInfoImpl;
	class TypeInfo
	{
	public:
		/// <summary>
		/// ��ȡһ��������Ϣ
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		static TypeInfo* info(const std::string& name);

	protected:
		/// <summary>
		/// ע��һ��������Ϣ
		/// </summary>
		/// <param name="type"></param>
		/// <returns></returns>
		static bool registerTypeInfo(TypeInfo* info);

	protected:
		/// <summary>
		/// ��ȡ���ʹ�С �����void�򷵻�0
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<typename T>
		static size_t Sizeof()
		{
			if constexpr (std::is_void_v<T>) return 0;
			else return sizeof(T);
		}

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

		/// <summary>
		/// ��ȡstd���͵�type_info
		/// </summary>
		/// <returns></returns>
		virtual const std::type_info& std_type_info() const = 0;

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
		/// ע�ᾲ̬��Ա��Ϣ
		/// </summary>
		/// <param name="info"></param>
		/// <returns></returns>
		bool registerStaticMember(const StaticMemberInfo& info);

		/// <summary>
		/// ��ȡ���о�̬��Ա��
		/// </summary>
		/// <returns></returns>
		std::list<std::string> staticMemberNames() const;

		/// <summary>
		/// ��ȡ��̬��Ա��Ϣ
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		std::optional<StaticMemberInfo> staticMember(const std::string& name) const;

		/// <summary>
		/// ע����ͨ��Ա��Ϣ
		/// </summary>
		/// <param name="info"></param>
		/// <returns></returns>
		bool registerNormalMember(const NormalMemberInfo& info);

		/// <summary>
		/// ��ȡ������ͨ��Ա��
		/// </summary>
		/// <returns></returns>
		std::list<std::string> normalMemberNames() const;

		/// <summary>
		/// ��ȡ��ͨ��Ա��Ϣ
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

		/// <summary>
		/// ע����ͨ������Ϣ
		/// </summary>
		/// <param name="info"></param>
		/// <returns></returns>
		bool registerNormalMethod(const NormalMethodInfo& info);

		/// <summary>
		/// ��ȡ������ͨ��������
		/// </summary>
		/// <returns></returns>
		std::unordered_set<std::string> normalMethodNames() const;

		/// <summary>
		/// ��ȡ��ͨ������Ϣ
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		std::list<NormalMethodInfo> normalMethod(const std::string& name) const;

	private:
		std::unique_ptr<TypeInfoImpl> m_impl{};
	};

	template<typename T>
	class RealTypeInfo : public TypeInfo { };

	/// <summary>
	/// ����ģ�����Ͱ����ض�Ӧ��Type����
	/// </summary>
	/// <typeparam name="...Args"></typeparam>
	/// <returns></returns>
	template<typename... Args>
	static std::list<TypeInfo*> unpackArgs() 
	{
		std::list<TypeInfo*> args;
		(args.push_back(RealTypeInfo<Args>::instance()), ...);
		return args;
	}

	/// <summary>
	/// ����ָ����̬����
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="F"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="name"></param>
	/// <param name="...args"></param>
	/// <returns></returns>
	template<typename T, typename F, typename... Args>
	static decltype(auto) invokeStaticMethod(const std::string& name, Args... args)
	{
		std::list<TypeInfo*> argList{ RTTR::unpackArgs<Args...>() };

		auto infos{ RealTypeInfo<T>::instance()->staticMethod(name) };
		if (auto info{ std::find_if(infos.begin(), infos.end(), [&argList](const auto& val) { return val.args == argList; }) }; info != infos.end())
			return info->invoke<F>(args...);
	}

	/// <summary>
	/// ����ָ����ͨ����
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="F"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="name"></param>
	/// <param name="o"></param>
	/// <param name="...args"></param>
	/// <returns></returns>
	template<typename T, typename F, typename... Args>
	static decltype(auto) invokeNormalMethod(const std::string& name, T* o, Args... args)
	{
		std::list<TypeInfo*> argList{ RTTR::unpackArgs<Args...>() };

		auto infos{ RealTypeInfo<T>::instance()->normalMethod(name) };
		if (auto info{ std::find_if(infos.begin(), infos.end(), [&argList](const auto& val) { return val.args == argList; }) }; info != infos.end())
			return info->invoke<F>(o, args...);
	}

	/// <summary>
	/// ��ȡָ����̬��Ա��ֵ
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="M"></typeparam>
	/// <param name="name"></param>
	/// <returns></returns>
	template<typename T, typename M>
	static decltype(auto) valueStaticMember(const std::string& name)
	{
		if (auto info{ RealTypeInfo<T>::instance()->staticMember(name) }; info.has_value())
			return info->value<M>();
	}

	/// <summary>
	/// ��ȡָ����ͨ��Ա��ֵ
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="M"></typeparam>
	/// <param name="name"></param>
	/// <param name="o"></param>
	/// <returns></returns>
	template<typename T, typename M>
	static decltype(auto) valueNormalMember(const std::string& name, T* o)
	{
		if (auto info{ RealTypeInfo<T>::instance()->normalMember(name) }; info.has_value())
			return info->value<T, M>(o);
	}
}

//ע������
#define RTTR_REGISTER(T) \
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
		std::string name() const override { return std_type_info().name(); } \
		size_t size() const override { return Sizeof<T>(); } \
		const std::type_info& std_type_info() const override { return typeid(T); } \
\
	private: \
		RealTypeInfo<T>() : TypeInfo() { registerTypeInfo(this); } \
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
#define RTTR_REGISTER_STATIC_MEMBER(Interview, Name) \
do \
{ \
	using T = std::remove_reference_t<decltype(*this)>; \
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
#define RTTR_REGISTER_STATIC_METHOD(Interview, R, Name, ...) \
do \
{ \
	using T = std::remove_reference_t<decltype(*this)>; \
	R (*address)(__VA_ARGS__){ T::Name }; \
	RTTR::RealTypeInfo<T>::instance()->registerStaticMethod({ #Name, Interview, RTTR::RealTypeInfo<R>::instance(), RTTR::unpackArgs<__VA_ARGS__>(), address }); \
} \
while (false)

//ע����ͨ����
#define RTTR_REGISTER_NORMAL_METHOD(Interview, R, Name, ...) \
do \
{ \
	using T = std::remove_reference_t<decltype(*this)>; \
	R (T::*address)(__VA_ARGS__){ &T::Name }; \
	RTTR::RealTypeInfo<T>::instance()->registerNormalMethod({ #Name, Interview, RTTR::RealTypeInfo<R>::instance(), RTTR::unpackArgs<__VA_ARGS__>(), *(void**)&address }); \
} \
while (false)
