#include <ez/meta.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <vector>
#include <iterator>

#include <fmt/core.h>

int main(int argc, char ** argv) {
	using container = std::vector<int>;
	using adaptor = std::back_insert_iterator<container>;

	using res0 = ez::iterator_value_t<container::iterator>;
	using res1 = ez::iterator_value_t<adaptor>;

	static_assert(std::is_same_v<res0, int>, "res0 is not the same!");
	static_assert(std::is_same_v<res1, int>, "res1 is not the same!");

	static constexpr bool res2 = ez::is_iterator_writable_v<adaptor, char>;
	static constexpr bool res3 = ez::is_iterator_writable_v<adaptor, short>;
	static constexpr bool res4 = ez::is_iterator_writable_v<adaptor, int>;
	static constexpr bool res5 = ez::is_iterator_writable_v<adaptor, const char*>;

	static_assert(res2 == true, "res2 is incorrect!");
	static_assert(res3 == true, "res3 is incorrect!");
	static_assert(res4 == true, "res4 is incorrect!");
	static_assert(res5 == false, "res5 is incorrect!");

	static constexpr bool res6 = ez::is_random_iterator<adaptor>::value;
	static constexpr bool res7 = ez::is_random_iterator<container::iterator>::value;
	static constexpr bool res8 = ez::is_random_iterator<int*>::value;
	static constexpr bool res9 = ez::is_random_iterator<int* const>::value;

	// the adaptor is not random access
	static_assert(res6 == false, "res6 is incorrect!");
	static_assert(res7 == true, "res7 is incorrect!");
	static_assert(res8 == true, "res8 is incorrect!");

	// This is a little weird, but its not technically a random access iterator because it cannot be incremented.
	// It would be easy to just copy the value, and then increment the copy, but that feels wrong.
	static_assert(res9 == false, "res9 is incorrect!");

	static_assert(ez::is_vec_v<float> == true, "ez::is_vec_v is incorrect");
	static_assert(ez::is_vec_v<double> == true, "ez::is_vec_v is incorrect");
	static_assert(ez::is_vec_v<int> == true, "ez::is_vec_v is incorrect");
	static_assert(ez::is_vec_v<glm::vec2> == true, "ez::is_vec_v is incorrect");
	static_assert(ez::is_vec_v<glm::vec3> == true, "ez::is_vec_v is incorrect");
	static_assert(ez::is_vec_v<glm::vec4> == true, "ez::is_vec_v is incorrect");

	static_assert(ez::is_mat_v<glm::vec2> == false, "ez::is_mat_v is incorrect");

	static_assert(ez::is_iterator_v<glm::vec2*>, "ez::is_iterator_v is incorrect");
	static_assert(ez::is_iterator_v<glm::vec2*const> == false, "ez::is_iterator_v is incorrect");
	static_assert(ez::is_iterator_v<const glm::vec2*> == true, "ez::is_iterator_v is incorrect");
	static_assert(ez::is_output_iterator_v<const glm::vec2*> == false, "ez::is_output_iterator_v is incorrect");

	fmt::print("Meta test is mostly compile time.\n");

	return 0;
}