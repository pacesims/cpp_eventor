#pragma once
#include <iostream>

#include "events.h"
#include "event_packs.h"

class A
{
	using errors = events::static_eventor<error_pack>;
public:
	A()
	{
		std::cout << "constructed a" << std::endl;
		token = errors::register_callback<error_types::TypeAError>([this](auto error) { catch_error_a(error); });
	}

	~A()
	{
		if(token) errors::un_register_callback<error_types::TypeAError>(token);
	}

	void catch_error_a(const error_types::TypeAError& error)
	{
		std::cout << "class a instance caught error a." << std::endl;
	}

	uint64_t token = -1;
};