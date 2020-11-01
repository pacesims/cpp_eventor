#pragma once
#include <type_traits>
#include <tuple>
#include <type_traits>
#include <any>

#include "type_pack.hpp"

namespace eventor::details
{
	template<typename T>
	struct packer;

	template<typename ... Ts>
	struct packer<std::tuple<Ts...>>
	{
		using type = typename tuple_unpack<std::tuple<Ts...>>::type_t;
	};

	template<typename ... Ts>
	struct packer<type_pack<Ts...>>
	{
		using type = type_pack<Ts...>;
	};
}

template<typename Pack>
class Eventor
{
	using EventPack = typename eventor::details::packer<Pack>::type;
public:
	template<typename Event>
	std::enable_if_t<EventPack::template index_t<Event>::value >= 0, uint64_t> register_callback(std::function<void(Event& type)> callback)
	{
		static uint64_t token = 0;
		token++;

		callbacks_[EventPack::template index_t<Event>::value].push_back({ token,[cb = std::move(callback)](std::any error)
		{
			Event& event_data = std::any_cast<Event&>(error);
			cb(event_data);
		} });

		return token;
	}

	template<typename Event>
	std::enable_if_t<EventPack:: template index_t<Event>::value >= 0> un_register_callback(uint64_t token)
	{
		callbacks_[EventPack:: template index_t<Event>::value].erase(
			std::remove_if(callbacks_[EventPack:: template index_t<Event>::value].begin(),
				callbacks_[EventPack:: template index_t<Event>::value].end(),
				[token](auto& pair)
				{
					return pair.first == token;
				}),
			callbacks_[EventPack:: template index_t<Event>::value].end());
	}

	template<typename Event>
	std::enable_if_t<EventPack:: template index_t<Event>::value >=0> raise_event(Event type)
	{
		for (auto& [token, cb] : callbacks_[EventPack:: template index_t<Event>::value])
		{
			cb(type);
		}
	}

private:
	std::map<int, std::vector<std::pair<uint64_t, std::function<void(std::any)>>>> callbacks_ = {};
};