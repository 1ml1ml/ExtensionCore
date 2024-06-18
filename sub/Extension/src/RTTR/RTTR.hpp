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

	struct NormalMemberInfo
	{
		std::string name;
		Interview interview{ None };
		Type* type{ nullptr };
		int offset{ -1 };
	};

	class TypeImpl;
	class Type
	{
	public:
		static Type* find(const std::string& name);

	protected:
		static inline std::unordered_map<std::string, Type*> s_types;

	public:
		Type();
		virtual ~Type();

	public:
		virtual std::string name() const = 0;
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
		std::optional<NormalMemberInfo> normalMember(const std::string & name) const;

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
		RealType<T>() : Type() { s_types[#T] = this; } \
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

//ע������ͨ��Ա����
#define RTTR_REGISTER_NORMAL_MEMBER(Interview, Name) \
do \
{ \
	using T = std::remove_reference<decltype(*this)>::type; \
	auto offset{ &T::Name }; \
	RTTR::RealType<T>::instance()->registerNormalMember({#Name, Interview, RTTR::RealType<decltype(T::Name)>::instance(), *(int*)(&offset)}); \
} \
while (false)

