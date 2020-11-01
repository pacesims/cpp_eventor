# cpp_eventor
A simple C++ Event system

Header only, so only step should be to include the Eventor header and start using. Events must be distinct types.

Example 1, as a base class:
```cpp
using namespace events;

class TypeAError { std::string name = "TYPEA"; };
class TypeBError { std::string name = "TYPEB"; };
class TypeCError { std::string name = "TYPEC"; };

using error_pack = std::tuple<TypeAError, TypeBError, TypeCError>;

class SomeSystemClass : public eventor<error_pack>
{
public:

	void test_raise()
	{
		raise_event(TypeAError{});
		raise_event(TypeBError{});
		raise_event(TypeCError{});
	}
};

int main()
{	
	SomeSystemClass sys;

	auto token = sys.register_callback<TypeAError>([](TypeAError& error)
		{
			std::cout << "Got Error A." << std::endl;
		});

	sys.test_raise();

	sys.un_register_callback<TypeAError>(token);

	sys.test_raise();
}
```

Example 2, Standalone:
```cpp
using namespace events;

class TypeAError { std::string name = "TYPEA"; };
class TypeBError { std::string name = "TYPEB"; };
class TypeCError { std::string name = "TYPEC"; };

using error_pack = std::tuple<TypeAError, TypeBError, TypeCError>;
using standalone_eventor = eventor<error_pack, false>; //We want to allow public access to raise, so pass in false

int main()
{	

	standalone_eventor eventor;

	auto e_token = eventor.register_callback<TypeAError>([](TypeAError& error)
		{
			std::cout << "Got Error A from standalone_eventor." << std::endl;
		});

	eventor.raise_event(TypeAError{});
}
```
Example 3, Multiple inheritance 
```cpp
using namespace events;

class TypeAError { std::string name = "TYPEA"; };
class TypeBError { std::string name = "TYPEB"; };
class TypeCError { std::string name = "TYPEC"; };

class EventA {};
class EventB {};
class EventC {};

using error_pack = std::tuple<TypeAError, TypeBError, TypeCError>;
using events = std::tuple<EventA, EventB, EventC>;

class SomeSystemClass : public eventor<error_pack>, public eventor<events>
{
public:
	using eventor<error_pack>::raise_event;
	using eventor<error_pack>::register_callback;
	using eventor<error_pack>::un_register_callback;

	using eventor<events>::raise_event;
	using eventor<events>::register_callback;
	using eventor<events>::un_register_callback;

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

int main()
{	
	SomeSystemClass sys;

	auto token = sys.register_callback<TypeAError>([](TypeAError& error)
		{
			std::cout << "Got Error A." << std::endl;
		});

	token = sys.register_callback<EventB>([](EventB& error)
		{
			std::cout << "Got Event B." << std::endl;
		});

	sys.test_raise();

	sys.un_register_callback<TypeAError>(token);

	sys.test_raise();
}
```
