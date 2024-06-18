#include <iostream>

#include "RTTR/RTTR.hpp"

struct A { };
RTTR_REGISTER_TYPE(A)

struct MyStruct : public A
{
public:
	using MapII = std::unordered_map<int, int>;

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

RTTR_REGISTER_TYPE(int)
RTTR_REGISTER_TYPE(std::string)
RTTR_REGISTER_TYPE(MyStruct::MapII)

int main()
{
	MyStruct{};

	auto type{ RTTR::Type::find("MyStruct") };
	std::cout << type->name() << ' ' << type->size() << ' ' << type->normalMember("map")->type->name() << std::endl;

	return 0;
}

inline MyStruct::MyStruct() : A()
{
	static std::once_flag of;
	std::call_once(of, [this]()
	{
		RRTR_REGISTER_SUPERCLASS(RTTR::Public, A);
		RTTR_REGISTER_NORMAL_MEMBER(RTTR::Public, a);
		RTTR_REGISTER_NORMAL_MEMBER(RTTR::Protected, b);
		RTTR_REGISTER_NORMAL_MEMBER(RTTR::Private, map);
	});
}
