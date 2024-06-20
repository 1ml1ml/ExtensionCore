#include <iostream>

#include "RTTR/RTTR.hpp"

RTTR_REGISTER(int)
RTTR_REGISTER(void)

struct ExampleSuperclass
{
public:
	static inline int s_i{ 100 };

public:
	static void func1(int)
	{
		std::cout << "func1_int" << std::endl;
	}

	static void func1(int, int)
	{
		std::cout << "func1_int_in" << std::endl;
	}

public:
	ExampleSuperclass();

public:
	virtual void func2(int)
	{
		std::cout << "func2_int" << std::endl;
	}

	void func2(int, int)
	{
		std::cout << "func2_int_int" << std::endl;
	}

public:
	int m_i{ 50 };
};
RTTR_REGISTER(ExampleSuperclass)

struct Example : public ExampleSuperclass
{
public:
	Example();

public:
	void func2(int) override
	{
		std::cout << "override func2_int" << std::endl;
	}
};
RTTR_REGISTER(Example)

int main()
{
	ExampleSuperclass es{};
	Example e{};

	auto esT{ RTTR::TypeInfo::find("struct ExampleSuperclass") };
	std::cout << esT->name() << " " << esT->size() << " " << std::endl;

	std::cout << esT->staticMember("s_i")->value<int>() << std::endl;
	std::cout << esT->normalMember("m_i")->value<ExampleSuperclass, int>(&es) << std::endl;

	esT->staticMethod("func1").front().call<void(*)(int)>(0);
	esT->staticMethod("func1").back().call<void(*)(int, int)>(0, 0);

	esT->normalMethod("func2").front().call<void(ExampleSuperclass::*)(int)>(&es, 0);
	esT->normalMethod("func2").back().call<void(ExampleSuperclass::*)(int, int)>(&es, 0, 0);

	auto eT{ RTTR::TypeInfo::find("struct Example") };
	eT->normalMethod("func2").front().call<void(Example::*)(int)>(&e, 0);

	esT = eT->superclass("struct ExampleSuperclass").value().info;
	std::cout << esT->name() << " " << esT->size() << " " << std::endl;

	return 0;
}

ExampleSuperclass::ExampleSuperclass()
{
	static std::once_flag of;
	std::call_once(of, [this]()
		{
			RTTR_REGISTER_STATIC_MEMBER(RTTR::Public, s_i);
			RTTR_REGISTER_NORMAL_MEMBER(RTTR::Public, m_i);

			RTTR_REGISTER_STATIC_METHOD(RTTR::Public, void, func1, int);
			RTTR_REGISTER_STATIC_METHOD(RTTR::Public, void, func1, int, int);

			RTTR_REGISTER_NORMAL_METHOD(RTTR::Public, void, func2, int);
			RTTR_REGISTER_NORMAL_METHOD(RTTR::Public, void, func2, int, int);
		});
}

Example::Example() : ExampleSuperclass()
{
	static std::once_flag of;
	std::call_once(of, [this]()
		{
			RRTR_REGISTER_SUPERCLASS(RTTR::Public, ExampleSuperclass);

			RTTR_REGISTER_NORMAL_METHOD(RTTR::Public, void, func2, int);
		});
}
