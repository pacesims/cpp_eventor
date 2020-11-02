#pragma once
#include <tuple>
#include <string>

namespace error_types
{
	class TypeAError { std::string name = "TYPEA"; };
	class TypeBError { std::string name = "TYPEB"; };
	class TypeCError { std::string name = "TYPEC"; };
}

namespace event_types
{
	class EventA {};
	class EventB {};
	class EventC {};
}

using namespace error_types;
using namespace event_types;

using error_pack = std::tuple<TypeAError, TypeBError, TypeCError>;
using normal_events = std::tuple<EventA, EventB, EventC>;