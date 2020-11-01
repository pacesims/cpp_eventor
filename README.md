# cpp_eventor
A simple C++ Event system

Example 1, as a base class:
```cpp
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
class TypeAError { std::string name = "TYPEA"; };
class TypeBError { std::string name = "TYPEB"; };
class TypeCError { std::string name = "TYPEC"; };

using error_pack = std::tuple<TypeAError, TypeBError, TypeCError>;
using standalone_eventor = Eventor<error_pack>;

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
