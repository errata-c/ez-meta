#pragma once
#include <type_traits>
#include <iterator>

namespace ez {
	// Check is the input type is a valid iterator.
	template<typename T, typename = int>
	struct is_iterator : std::false_type {};
	template<typename T>
	struct is_iterator < T, decltype(typename std::iterator_traits<T>::iterator_category{}, 0) > : std::true_type {};
	template<typename T>
	static constexpr bool is_iterator_v = is_iterator<T>::value;

	// Finds the iterator category of the input type, if there is one.
	// This accepts only types that have an actual iterator_category, or a pointer type.
	template<typename T, typename = int>
	struct extract_iterator_category {
		// This assertion will always trigger if no specializations are active.
		static_assert(is_iterator_v<T>, "ez::extract_iterator_category called with a non-iterator type!");
	};
	// Pointer types are always valid random access iterators
	template<typename T>
	struct extract_iterator_category<T*, int> {
		using type = typename std::random_access_iterator_tag;
	};
	// We honor the iterator category defined by the input type, hoping that it is correct...
	template<typename T>
	struct extract_iterator_category<T, decltype(std::declval<typename T::iterator_category>(), 0)> {
		using type = typename T::iterator_category;
	};
	template<typename T>
	using extract_iterator_category_t = typename extract_iterator_category<T>::type;


	// Extracts normal iterator value_type
	template<typename Iter, typename = int>
	struct extract_iterator_value_type {
		using type = typename Iter::value_type;
	};
	// Extracts pointer iterator value_type
	template<typename Iter>
	struct extract_iterator_value_type<Iter*, int> {
		using type = Iter;
	};
	// Extracts iterator adaptor value_type, like from std::back_insert_iterator
	template<typename Iter>
	struct extract_iterator_value_type<Iter, decltype(std::declval<typename Iter::container_type>(), 0)> {
		using container_type = typename Iter::container_type;
		using type = typename container_type::value_type;
	};
	template<typename Iter>
	using iterator_value_t = typename extract_iterator_value_type<Iter>::type;
	template<typename Iter>
	using iterator_reference_t = iterator_value_t<Iter>&;
	template<typename Iter>
	using iterator_pointer_t = iterator_value_t<Iter>*;


	// Check if the input value_t can be converted to the iterator value_type
	template<typename iter, typename value_t>
	struct is_iterator_writable {
		using iter_value_t = iterator_value_t<iter>;
		static constexpr bool value = std::is_convertible_v<value_t, iter_value_t> && !std::is_const_v<iter_value_t>;
	};
	template<typename iter, typename value_t>
	inline constexpr bool is_iterator_writable_v = is_iterator_writable<iter, value_t>::value;


	// Check if the iterator type returns a type convertible to the value_t
	template<typename iter, typename value_t>
	struct is_iterator_readable {
		using iter_value_t = iterator_value_t<iter>;
		static constexpr bool value = std::is_convertible_v<iter_value_t, value_t>;
	};
	template<typename iter, typename value_t>
	inline constexpr bool is_iterator_readable_v = is_iterator_readable<iter, value_t>::value;


	// Check if iterator meets the requirements for random_access_iterator_tag
	template<typename iter>
	struct is_random_iterator {
		static constexpr bool value = std::is_same_v<extract_iterator_category_t<iter>, typename std::random_access_iterator_tag>;
	};
	template<typename iter>
	inline constexpr bool is_random_iterator_v = is_random_iterator<iter>::value;


	// Check if iterator meets the requirements for output_iterator_tag
	template<typename iter, typename = int>
	struct is_output_iterator : std::false_type {};
	template<typename iter>
	struct is_output_iterator<iter*, int> {
		static constexpr bool value = !std::is_const_v<iter>;
	};
	template<typename iter>
	struct is_output_iterator<iter, decltype(std::declval<typename iter::iterator_category>(), 0)> {
		static constexpr bool value = !std::is_same_v<typename iter::iterator_category, typename std::input_iterator_tag>;
	};
	template<typename iter>
	inline constexpr bool is_output_iterator_v = is_output_iterator<iter>::value;


	// Check if iterator meets the requirements for bidirectional_iterator_tag
	template<typename iter, typename = int>
	struct is_bidirectional_iterator : std::false_type {};
	template<typename iter>
	struct is_bidirectional_iterator<iter*, int> : std::true_type {};
	template<typename iter>
	struct is_bidirectional_iterator<iter, decltype(std::declval<typename iter::iterator_category>(), 0)> {
		static constexpr bool value =
			std::is_same_v<typename iter::iterator_category, typename std::random_access_iterator_tag> ||
			std::is_same_v<typename iter::iterator_category, typename std::bidirectional_iterator_tag>;
	};
	template<typename iter>
	inline constexpr bool is_bidirectional_iterator_v = is_bidirectional_iterator<iter>::value;


	// Check if iterator meets the requirements for forward_iterator_category
	template<typename iter, typename = int>
	struct is_forward_iterator : std::false_type {};
	template<typename iter>
	struct is_forward_iterator<iter*, int> : std::true_type {};
	template<typename iter>
	struct is_forward_iterator<iter, decltype(std::declval<typename iter::iterator_category>(), 0)> {
		static constexpr bool value =
			!std::is_same_v<typename iter::iterator_category, typename std::input_iterator_tag> &&
			!std::is_same_v<typename iter::iterator_category, typename std::output_iterator_tag>;
	};
	template<typename iter>
	inline constexpr bool is_forward_iterator_v = is_forward_iterator<iter>::value;


	// Check if iterator meets the requirements for input_iterator_category
	template<typename iter>
	struct is_input_iterator {
		static constexpr bool value = !std::is_same_v<extract_iterator_category_t<iter>, typename std::output_iterator_tag>;
	};
	template<typename iter>
	inline constexpr bool is_input_iterator_v = is_input_iterator<iter>::value;

	// Check if the type is an iterable type, ie. it has a begin() and an end() method returning iterators.
	template<typename T, typename = int>
	struct is_iterable: std::false_type {};
	// Matches when the type has both a begin() and an end() method defined.
	template<typename T>
	struct is_iterable<T, decltype(std::declval<T>().begin(), std::declval<T>().end(), 0)> {
		// Check to see if the types returned by the begin() and end() methods are actually iterators.
		static constexpr bool value =
			is_iterator_v<decltype(std::declval<T>().begin())> &&
			is_iterator_v<decltype(std::declval<T>().end())>;
	};
	template<typename T>
	inline constexpr bool is_iterable_v = is_iterable<T>::value;


}