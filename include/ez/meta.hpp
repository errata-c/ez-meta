#pragma once
#include <type_traits>
#include "meta/iterator.hpp"
#include <glm/fwd.hpp>

namespace ez {
	// Determine if the input type T is a valid vector type.
	// Returns true for all arithmetic types and glm::vec types
	template<typename T>
	struct is_vec: std::is_arithmetic<T> {};
	template<typename T, glm::length_t N>
	struct is_vec<glm::vec<N, T>> : std::is_arithmetic<T> {};
	template<typename T>
	inline constexpr bool is_vec_v = is_vec<T>::value;

	template<typename T>
	struct is_vec_strict : std::false_type {};
	template<typename T, glm::length_t N>
	struct is_vec_strict<glm::vec<N, T>> : std::is_arithmetic<T> {};
	template<typename T>
	inline constexpr bool is_vec_strict_v = is_vec_strict<T>::value;

	template<typename T>
	struct is_real_vec : std::is_floating_point<T> {};
	template<typename T, glm::length_t N>
	struct is_real_vec<glm::vec<N, T>>: std::is_floating_point<T> {};
	template<typename T>
	inline constexpr bool is_real_vec_v = is_real_vec<T>::value;
	

	template<typename T>
	struct vec_traits {
		static_assert(is_vec_v<T>, "typename passed into vec_traits must be a vector type!");
		using value_type = T;
		static constexpr std::size_t length = 1;
	};
	template<typename T, glm::length_t N>
	struct vec_traits<glm::vec<N, T>> {
		static_assert(is_vec_v<T>, "typename passed into vec_traits must be a vector type!");
		using value_type = T;
		static constexpr std::size_t length = N;
	};

	template<typename T>
	using vec_value_t = typename vec_traits<T>::value_type;

	template<typename T>
	inline constexpr std::size_t vec_length_v = vec_traits<T>::length;

	template<typename T, typename = int>
	struct is_mat: std::is_arithmetic<T> {};
	template<typename T, glm::length_t M, glm::length_t N>
	struct is_mat<glm::mat<M, N, T>>: std::is_arithmetic<T> {};
	template<typename T>
	inline constexpr bool is_mat_v = is_mat<T>::value;

	template<typename T, typename = int>
	struct is_mat_strict: std::false_type {};
	template<typename T, glm::length_t M, glm::length_t N>
	struct is_mat_strict<glm::mat<M, N, T>>: std::is_arithmetic<T> {};
	template<typename T>
	inline constexpr bool is_mat_strict_v = is_mat_strict<T>::value;
}