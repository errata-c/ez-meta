#pragma once
#include <type_traits>
#include <iterator>
#include <glm/fwd.hpp>

namespace ez {
	template<typename T, typename = int>
	struct is_iterator : std::false_type {};
	template<typename T>
	struct is_iterator < T, decltype(std::declval<typename T::iterator_category>(), 0) > : std::true_type {};
	template<typename T>
	struct is_iterator < T*, int > : std::true_type {};
	template<typename T>
	static constexpr bool is_iterator_v = is_iterator<T>::value;

	template<typename T, typename = int>
	struct extract_iterator_category {
		using type = void;
	};
	template<typename T>
	struct extract_iterator_category<T*, int> {
		using type = std::random_access_iterator_tag;
	};
	template<typename T>
	struct extract_iterator_category<T, decltype(std::declval<typename T::iterator_category>(), 0)>	{
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


	// Can we write this value type to the iterator?
	template<typename iter, typename value_t>
	struct is_iterator_writable {
		using iter_value_t = iterator_value_t<iter>;
		static constexpr bool value = std::is_convertible_v<value_t, iter_value_t> && !std::is_const_v<iter_value_t>;
	};
	template<typename iter, typename value_t>
	inline constexpr bool is_iterator_writable_v = is_iterator_writable<iter, value_t>::value;


	// Can we read this value type from the iterator?
	template<typename iter, typename value_t>
	struct is_iterator_readable {
		using iter_value_t = iterator_value_t<iter>;
		static constexpr bool value = std::is_convertible_v<iter_value_t, value_t>;
	};
	template<typename iter, typename value_t>
	inline constexpr bool is_iterator_readable_v = is_iterator_readable<iter, value_t>::value;



	template<typename iter>
	struct is_random_iterator {
		static constexpr bool value = std::is_same_v<ez::extract_iterator_category_t<iter>, typename std::random_access_iterator_tag>;
	};
	template<typename iter>
	inline constexpr bool is_random_iterator_v = is_random_iterator<iter>::value;


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



	template<typename iter>
	struct is_input_iterator {
		static constexpr bool value = !std::is_same_v<extract_iterator_category_t<iter>, typename std::output_iterator_tag>;
	};
	template<typename iter>
	inline constexpr bool is_input_iterator_v = is_input_iterator<iter>::value;


	template<typename T, typename = int>
	struct is_vec: std::is_arithmetic<T> {};
	template<typename T>
	struct is_vec<T, decltype(std::declval<typename T::length_type>(), 0)> {
		using value_type = typename T::value_type;
		static constexpr bool value = std::is_same_v<T, glm::vec<T::length(), value_type>>;
	};
	template<typename T>
	inline constexpr bool is_vec_v = is_vec<T>::value;


	template<typename T, typename = int>
	struct is_real_vec : std::is_floating_point<T> {};
	template<typename T>
	struct is_real_vec<T, decltype(std::declval<typename T::length_type>(), 0)> {
		using value_type = typename T::value_type;
		static constexpr bool value = 
			std::is_same_v<T, glm::vec<T::length(), value_type>> &&
			std::is_floating_point_v<value_type>;
	};
	template<typename T>
	inline constexpr bool is_real_vec_v = is_real_vec<T>::value;
	

	template<typename T>
	struct vec_traits {
		using value_type = T;
		static constexpr std::size_t length = 1;
	};
	template<typename T>
	struct vec_traits<typename glm::vec<2, T>> {
		using value_type = T;
		static constexpr std::size_t length = 2;
	};
	template<typename T>
	struct vec_traits<typename glm::vec<3, T>> {
		using value_type = T;
		static constexpr std::size_t length = 3;
	};
	template<typename T>
	struct vec_traits<typename glm::vec<4, T>> {
		using value_type = T;
		static constexpr std::size_t length = 4;
	};

	template<typename T>
	using vec_value_t = typename vec_traits<T>::value_type;

	template<typename T>
	inline constexpr std::size_t vec_length_v = vec_traits<T>::length;


	template<typename T, typename = int>
	struct is_mat : std::is_arithmetic<T> {};
	template<typename T>
	struct is_mat<T, decltype(std::declval<typename T::length_type>(), std::declval<typename T::col_type>(), 0)> {
		using value_type = typename T::value_type;
		using col_type = typename T::col_type;

		static constexpr bool value = std::is_same_v<T, glm::mat<T::length(), col_type::length(), value_type>>;
	};
	template<typename T>
	inline constexpr bool is_mat_v = is_mat<T>::value;
}