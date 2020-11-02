#include <iostream>
#include <functional>
#include <map>

#include "event_packs.h"
#include "events.h"


#include "test.h"

using namespace events;

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

using static_errors = static_eventor<error_pack>;

using standalone_events = eventor<error_pack, false>;

int main()
{	
	{
		std::vector<A> listeners(3);
		static_errors::raise_event(TypeAError{});
	}

	SomeSystemClass sys;

	static_errors::register_callback<TypeAError>([](const TypeAError& error)
		{
			std::cout << "Got Static Error A." << std::endl;
		});

	static_errors::raise_event(TypeAError{});

	standalone_events events;

	events.raise_event(TypeAError{});

	auto token_a = sys.register_callback<TypeAError>([](const TypeAError& error)
		{
			std::cout << "Got Error A." << std::endl;
		});

	auto token_b = sys.register_callback<EventB>([](const EventB& error)
		{
			std::cout << "Got Event B." << std::endl;
		});

	sys.test_raise();

	sys.un_register_callback<TypeAError>(token_a);

	sys.test_raise();
}