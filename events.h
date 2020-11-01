#pragma once
#include <type_traits>
#include <tuple>
#include <type_traits>
#include <any>

#include "type_pack.hpp"

namespace events
{
	namespace details
	{
		template<typename T>
		struct type_pack_conversion;

		template<typename ... Ts>
		struct type_pack_conversion<std::tuple<Ts...>>
		{
			using type = typename tuple_to_type_pack<std::tuple<Ts...>>::type;
		};

		template<typename ... Ts>
		struct type_pack_conversion<type_pack<Ts...>>
		{
			using type = type_pack<Ts...>;
		};
	}

	template<typename Pack, bool protected_raise = true>
	class eventor
	{
		template<typename Event>
		using index_t = typename details::type_pack_conversion<Pack>::type::template index_t<Event>;

	public:
		template<typename Event>
		std::enable_if_t<index_t<Event>::value >= 0, uint64_t> register_callback(std::function<void(const Event& type)> callback)
		{
			static uint64_t token = 0;
			token++;

			callbacks_[index_t<Event>::value].push_back({ token,[cb = std::move(callback)](std::any error)
			{
				const Event& event_data = std::any_cast<const Event>(error);
				cb(event_data);
			} });

			return token;
		}

		template<typename Event>
		std::enable_if_t<index_t<Event>::value >= 0> un_register_callback(uint64_t token)
		{
			callbacks_[index_t<Event>::value].erase(
				std::remove_if(callbacks_[index_t<Event>::value].begin(),
					callbacks_[index_t<Event>::value].end(),
					[token](auto& pair)
					{
						return pair.first == token;
					}),
				callbacks_[index_t<Event>::value].end());
		}

		template<typename Event>
		std::enable_if_t<index_t<Event>::value >= 0 && !protected_raise> raise_event(const Event& type)
		{
			for (auto& [token, cb] : callbacks_[index_t<Event>::value])
			{
				cb(type);
			}
		}

	protected:

		template<typename Event>
		std::enable_if_t<index_t<Event>::value >= 0 && protected_raise> raise_event(const Event& type)
		{
			for (auto& [token, cb] : callbacks_[index_t<Event>::value])
			{
				cb(type);
			}
		}

	private:
		std::map<int, std::vector<std::pair<uint64_t, std::function<void(std::any)>>>> callbacks_ = {};
	};
}