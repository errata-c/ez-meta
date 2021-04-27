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

	using res0 = ez::iterator_value_t<typename container::iterator>;
	using res1 = ez::iterator_value_t<adaptor>;

	static_assert(std::is_same_v<res0, int>, "res0 is not the same!");
	static_assert(std::is_same_v<res1, int>, "res1 is not the same!");

	static_assert(ez::is_iterator_writable_v<adaptor, char>,		 "ez::is_iterator_writable_v is incorrect!");
	static_assert(ez::is_iterator_writable_v<adaptor, short>,		 "ez::is_iterator_writable_v is incorrect!");
	static_assert(ez::is_iterator_writable_v<adaptor, int>,			 "ez::is_iterator_writable_v is incorrect!");
	static_assert(!ez::is_iterator_writable_v<adaptor, const char*>, "ez::is_iterator_writable_v is incorrect!");

	// the adaptor is not random access
	static_assert(!ez::is_random_iterator_v<adaptor>,						"ez::is_random_iterator_v is incorrect!");
	static_assert(ez::is_random_iterator_v<typename container::iterator>,	"ez::is_random_iterator_v is incorrect!");
	static_assert(ez::is_random_iterator_v<int*>,							"ez::is_random_iterator_v is incorrect!");

	// Not a valid iterator, we cannot increment the pointer type.
	static_assert(!ez::is_iterator_v<int* const>, "ez::is_iterator_v is incorrect!");

	static_assert(ez::is_vec_v<float>,		"ez::is_vec_v is incorrect");
	static_assert(ez::is_vec_v<double>,		"ez::is_vec_v is incorrect");
	static_assert(ez::is_vec_v<int>,		"ez::is_vec_v is incorrect");
	static_assert(ez::is_vec_v<glm::vec2>,	"ez::is_vec_v is incorrect");
	static_assert(ez::is_vec_v<glm::vec3>,	"ez::is_vec_v is incorrect");
	static_assert(ez::is_vec_v<glm::vec4>,	"ez::is_vec_v is incorrect");

	static_assert(!ez::is_mat_v<glm::vec2>, "ez::is_mat_v is incorrect");

	static_assert(ez::is_iterator_v<glm::vec2*>,						"ez::is_iterator_v is incorrect");
	static_assert(ez::is_iterator_v<glm::vec2*const> == false,			"ez::is_iterator_v is incorrect");
	static_assert(ez::is_iterator_v<const glm::vec2*> == true,			"ez::is_iterator_v is incorrect");
	static_assert(ez::is_output_iterator_v<const glm::vec2*> == false,	"ez::is_output_iterator_v is incorrect");

	fmt::print("Meta test is mostly compile time.\n");

	return 0;
}