#include <iostream>

#include "RTTR/RTTR.hpp"

struct A
{

};
RTTR_REGISTER_TYPE(A)

struct MyStruct : public A
{
public:
	using MapII = std::unordered_map<int, int>;
	MyStruct();

private:
	int a{ 0 };
	std::string b{};
	MapII map;
};
RTTR_REGISTER_TYPE(MyStruct)

RTTR_REGISTER_TYPE(int)
RTTR_REGISTER_TYPE(std::string)
RTTR_REGISTER_TYPE(MyStruct::MapII)

int main()
{
	MyStruct{};
	MyStruct{};
	MyStruct{};
	MyStruct{};
	MyStruct{};

	auto t1{ RTTR::Type::find("MyStruct") };
	std::cout << t1->name() << ' ' << t1->size() << ' ' << t1->normalMember("map")->type->name() << std::endl;

	return 0;
}

inline MyStruct::MyStruct() : A()
{
	static std::once_flag of;
	std::call_once(of, [this]()
	{
		RRTR_REGISTER_SUPERCLASS(RTTR::Public, A);
		RTTR_REGISTER_NORMAL_MEMBER(RTTR::Private, a);
		RTTR_REGISTER_NORMAL_MEMBER(RTTR::Private, b);
		RTTR_REGISTER_NORMAL_MEMBER(RTTR::Private, map);
	});
}
