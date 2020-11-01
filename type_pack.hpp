#pragma once
#include <tuple>
#include <type_traits>

template<typename What, typename ... Args>
struct is_present {
	static constexpr bool value{ (std::is_same_v<What, Args> || ...) };
};

template <class T, class Tuple>
struct Index;

template <class T, class... Types>
struct Index<T, std::tuple<T, Types...>> {
	static const int value = 0;
};

template <class T, class U, class... Types>
struct Index<T, std::tuple<U, Types...>> {

	template<typename What, typename ... Looking>
	static constexpr int Value()
	{
		if constexpr (is_present<What, Looking...>::value)
		{
			return 1 + Index<What, std::tuple<Looking...>>::value;
		}
		else
		{
			return -1;
		}
	}

	static constexpr int value = Value<T, Types...>();
};

template <class... Args>
struct type_pack
{
	template <std::size_t N>
	using type_t = typename std::tuple_element<N, std::tuple<Args...>>::type;

	template<typename T>
	using index_t = Index<T, std::tuple<Args...>>;
};

template<typename Tuple, int index = std::tuple_size_v<Tuple>-1, typename ... Types>
struct tuple_unpack
{
	using type_t = typename tuple_unpack<Tuple, index - 1, std::tuple_element_t<index, Tuple>, Types...>::type_t;
};

template<typename Tuple, typename ... Types>
struct tuple_unpack<Tuple, -1, Types...>
{
	using type_t = type_pack<Types...>;
};