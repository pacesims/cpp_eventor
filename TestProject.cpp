#include <iostream>
#include <functional>
#include <map>

#include "events.h"

using namespace events;

class TypeAError { std::string name = "TYPEA"; };
class TypeBError { std::string name = "TYPEB"; };
class TypeCError { std::string name = "TYPEC"; };

class EventA {};
class EventB {};
class EventC {};

using error_pack = std::tuple<TypeAError, TypeBError, TypeCError>;
using normal_events = std::tuple<EventA, EventB, EventC>;

class SomeSystemClass : public eventor<error_pack>, public eventor<normal_events>
{
public:
	using eventor<error_pack>::raise_event;
	using eventor<error_pack>::register_callback;
	using eventor<error_pack>::un_register_callback;

	using eventor<normal_events>::raise_event;
	using eventor<normal_events>::register_callback;
	using eventor<normal_events>::un_register_callback;

	void test_raise()
	{
		raise_event(TypeAError{});
		raise_event(TypeBError{});
		raise_event(TypeCError{});

		raise_event(EventA{});
		raise_event(EventB{});
		raise_event(EventC{});
	}
};


using standalone_events = eventor<error_pack, false>;
int main()
{	
	SomeSystemClass sys;

	standalone_events events;

	events.raise_event(TypeAError{});

	auto token = sys.register_callback<TypeAError>([](const TypeAError& error)
		{
			std::cout << "Got Error A." << std::endl;
		});

	token = sys.register_callback<EventB>([](const EventB& error)
		{
			std::cout << "Got Event B." << std::endl;
		});

	sys.test_raise();

	sys.un_register_callback<TypeAError>(token);

	sys.test_raise();
}