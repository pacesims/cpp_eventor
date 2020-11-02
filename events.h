#pragma once
#include <type_traits>
#include <tuple>
#include <type_traits>
#include <any>

namespace events
{
	namespace details
	{
		template<typename T, typename ... Ts>
		struct is_present {
			static constexpr bool value{ (std::is_same_v<T, Ts> || ...) };
		};

		template <class T, class Tuple>
		struct tuple_index;

		template <class T>
		struct tuple_index<T, std::tuple<>> {
			static const int16_t value = -1;
		};

		template <class T, class... Types>
		struct tuple_index<T, std::tuple<T, Types...>> {
			static const int16_t value = 0;
		};

		template <class T, class U, class ... Types>
		struct tuple_index<T, std::tuple<U, Types...>> {

			static constexpr int value = std::conditional_t<is_present<T, Types...>::value,
				std::integral_constant<int16_t, 1 + tuple_index<T, std::tuple<Types...>>::value>,
				std::integral_constant<int16_t, -1>>();
		};

		template<typename T>
		struct is_tuple : std::false_type {};

		template<typename ... Ts>
		struct is_tuple<std::tuple<Ts...>> : std::true_type {};
	}

	template<typename Pack, bool protected_raise = true>
	class eventor
	{
		static_assert(details::is_tuple<Pack>::value, "Pack must be a tuple of event types.");

		template <std::size_t N>
		using type = typename std::tuple_element<N, Pack>::type;

		template<typename T>
		using index_t = details::tuple_index<T, Pack>;

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

	template<typename Pack>
	class static_eventor
	{
		static_assert(details::is_tuple<Pack>::value, "Pack must be a tuple of event types.");

		template <std::size_t N>
		using type = typename std::tuple_element<N, Pack>::type;

		template<typename T>
		using index_t = details::tuple_index<T, Pack>;

	public:
		template<typename Event>
		inline static std::enable_if_t<index_t<Event>::value >= 0, uint64_t> register_callback(std::function<void(const Event& type)> callback)
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
		inline static std::enable_if_t<index_t<Event>::value >= 0> un_register_callback(uint64_t token)
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
		inline static std::enable_if_t<index_t<Event>::value >= 0> raise_event(const Event& type)
		{
			for (auto& [token, cb] : callbacks_[index_t<Event>::value])
			{
				cb(type);
			}
		}

	private:
		inline static std::map<int, std::vector<std::pair<uint64_t, std::function<void(std::any)>>>> callbacks_ = {};
	};
}