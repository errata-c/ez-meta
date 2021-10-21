#pragma once
#include <type_traits>
#include "meta/iterator.hpp"
#include <glm/fwd.hpp>

namespace ez {
	// Determine if the input type T is a valid vector type.
	// Returns true for all arithmetic types and 
	template<typename T, std::size_t N = 1>
	struct is_vec: std::is_arithmetic<T> {};
	template<typename T, std::size_t N>
	struct is_vec<glm::vec<N, T>> : std::is_arithmetic<T> {};
	template<typename T>
	inline constexpr bool is_vec_v = is_vec<T>::value;

	template<typename T, std::size_t N = 1>
	struct is_vec_strict : std::false_type {};
	template<typename T, std::size_t N>
	struct is_vec_strict<glm::vec<N, T>> : std::is_arithmetic<T> {};
	template<typename T>
	inline constexpr bool is_vec_strict_v = is_vec_strict<T>::value;


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
		static_assert(is_vec_v<T>, "typename passed into vec_traits must be a glm vector type!");
		using value_type = typename T::value_type;
		static constexpr std::size_t length = T::length();
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