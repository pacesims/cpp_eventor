#pragma once
#include <tuple>
#include <type_traits>

template<typename What, typename ... Args>
struct is_present {
	static constexpr bool value{ (std::is_same_v<What, Args> || ...) };
};

template <class T, class Tuple>
struct tuple_index;

template <class T>
struct tuple_index<T, std::tuple<>> {
	static const int value = -1;
};

template <class T, class... Types>
struct tuple_index<T, std::tuple<T, Types...>> {
	static const int value = 0;
};

template <class T, class U, class ... Types>
struct tuple_index<T, std::tuple<U, Types...>> {

	static constexpr int value = std::conditional_t<is_present<T, Types...>::value, 
		std::integral_constant<int, 1 + tuple_index<T, std::tuple<Types...>>::value>,
		std::integral_constant<int, -1>>();
};

template <class... Args>
struct type_pack
{
	template <std::size_t N>
	using type = typename std::tuple_element<N, std::tuple<Args...>>::type;

	template<typename T>
	using index_t = tuple_index<T, std::tuple<Args...>>;
};

template<typename Tuple, int index = std::tuple_size_v<Tuple>-1, typename ... Types>
struct tuple_to_type_pack
{
	using type = typename tuple_to_type_pack<Tuple, index - 1, std::tuple_element_t<index, Tuple>, Types...>::type;
};

template<typename Tuple, typename ... Types>
struct tuple_to_type_pack<Tuple, -1, Types...>
{
	using type = type_pack<Types...>;
};