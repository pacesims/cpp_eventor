#include <iostream>
#include <functional>
#include <map>

#include "type_pack.hpp"
#include "Eventor.h"

class TypeAError { std::string name = "TYPEA"; };
class TypeBError { std::string name = "TYPEB"; };
class TypeCError { std::string name = "TYPEC"; };

using error_pack = std::tuple<TypeAError, TypeBError, TypeCError>;

class SomeSystemClass : public Eventor<error_pack>
{
public:

	void test_raise()
	{
		raise_event(TypeAError{});
		raise_event(TypeBError{});
		raise_event(TypeCError{});
	}
};

using standalone_eventor = Eventor<error_pack>;

int main()
{	
	SomeSystemClass sys;
	standalone_eventor eventor;

	auto e_token = eventor.register_callback<TypeAError>([](TypeAError& error)
		{
			std::cout << "Got Error A from standalone_eventor." << std::endl;
		});

	eventor.raise_event(TypeAError{});

	auto token = sys.register_callback<TypeAError>([](TypeAError& error)
		{
			std::cout << "Got Error A." << std::endl;
		});

	sys.test_raise();

	sys.un_register_callback<TypeAError>(token);

	sys.test_raise();
}