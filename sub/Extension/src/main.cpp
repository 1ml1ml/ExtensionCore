#include <iostream>

#include "RTTR/RTTR.hpp"

struct A { };
RTTR_REGISTER_TYPE(A)

class MyStruct : public A
{
public:
	using MapII = std::unordered_map<int, int>;

public:
	static inline double s_d;

public:
	static int s_test(int, int, int, int) { return 0; }
	static int test() { return 0; }

public:
	MyStruct();

public:
	int a{ 0 };

protected:
	std::string b{};

private:
	MapII map;
};
RTTR_REGISTER_TYPE(MyStruct)

RTTR_REGISTER_TYPE(void)
RTTR_REGISTER_TYPE(double)
RTTR_REGISTER_TYPE(int)
RTTR_REGISTER_TYPE(std::string)
RTTR_REGISTER_TYPE(MyStruct::MapII)

int main()
{
	MyStruct{};

	auto type{ RTTR::TypeInfo::find("MyStruct") };
	std::cout << type->name() << ' ' << type->size() << ' ' << type->normalMember("map")->info->name() << std::endl;

	return 0;
}

inline MyStruct::MyStruct() : A()
{
	RTTR_REGISTER_STATIC_METHOD(MyStruct, RTTR::Public, s_test, int, int, int, int);

	static std::once_flag of;
	std::call_once(of, [this]()
	{
		RRTR_REGISTER_SUPERCLASS(RTTR::Public, A);

		RTTR_REGISTER_STATIC_MEMBER(MyStruct, RTTR::Public, s_d);

		RTTR_REGISTER_NORMAL_MEMBER(RTTR::Public, a);
		RTTR_REGISTER_NORMAL_MEMBER(RTTR::Protected, b);
		RTTR_REGISTER_NORMAL_MEMBER(RTTR::Private, map);
	});
}
