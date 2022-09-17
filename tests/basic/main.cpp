#include <ez/meta.hpp>

#include <glm/gtc/vec1.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include <array>
#include <vector>
#include <iterator>
#include <cassert>

#include <fmt/core.h>


using container = std::vector<int>;
using adaptor = std::back_insert_iterator<container>;

using res0 = ez::iterator_value_t<typename container::iterator>;
using res1 = ez::iterator_value_t<adaptor>;

static_assert(std::is_same_v<res0, int>, "res0 is not the same!");
static_assert(std::is_same_v<res1, int>, "res1 is not the same!");

static_assert(ez::is_iterator_writable_v<adaptor, char>, "ez::is_iterator_writable_v is incorrect!");
static_assert(ez::is_iterator_writable_v<adaptor, short>, "ez::is_iterator_writable_v is incorrect!");
static_assert(ez::is_iterator_writable_v<adaptor, int>, "ez::is_iterator_writable_v is incorrect!");
static_assert(!ez::is_iterator_writable_v<adaptor, const char*>, "ez::is_iterator_writable_v is incorrect!");

// the adaptor is not random access
static_assert(!ez::is_random_iterator_v<adaptor>, "ez::is_random_iterator_v is incorrect!");
static_assert(ez::is_random_iterator_v<typename container::iterator>, "ez::is_random_iterator_v is incorrect!");
static_assert(ez::is_random_iterator_v<int*>, "ez::is_random_iterator_v is incorrect!");

// Not a valid iterator, we cannot increment the pointer type.
static_assert(!ez::is_iterator_v<int* const>, "ez::is_iterator_v is incorrect!");

static_assert(ez::is_vec_v<float>, "ez::is_vec_v is incorrect");
static_assert(ez::is_vec_v<double>, "ez::is_vec_v is incorrect");
static_assert(ez::is_vec_v<int>, "ez::is_vec_v is incorrect");

static_assert(ez::is_real_vec_v<float>, "ez::is_real_vec_v is incorrect");
static_assert(ez::is_real_vec_v<double>, "ez::is_real_vec_v is incorrect");
static_assert(ez::is_real_vec_v<glm::vec2>, "ez::is_real_vec_v is incorrect");
static_assert(ez::is_real_vec_v<glm::vec3>, "ez::is_real_vec_v is incorrect");
static_assert(ez::is_real_vec_v<glm::vec4>, "ez::is_real_vec_v is incorrect");
static_assert(!ez::is_real_vec_v<int>, "ez::is_real_vec_v is incorrect");

static_assert(!ez::is_vec_strict_v<float>, "ez::is_vec_strict_v is incorrect");
static_assert(!ez::is_vec_strict_v<double>, "ez::is_vec_strict_v is incorrect");
static_assert(!ez::is_vec_strict_v<int>, "ez::is_vec_strict_v is incorrect");

static_assert(ez::is_vec_v<glm::vec1>, "ez::is_vec_v is incorrect");
static_assert(ez::is_vec_v<glm::vec2>, "ez::is_vec_v is incorrect");
static_assert(ez::is_vec_v<glm::vec3>, "ez::is_vec_v is incorrect");
static_assert(ez::is_vec_v<glm::vec4>, "ez::is_vec_v is incorrect");

static_assert(!ez::is_mat_v<glm::vec2>, "ez::is_mat_v is incorrect");
static_assert(ez::is_mat_v<glm::mat2>, "ez::is_mat_v is incorrect");
static_assert(ez::is_mat_v<glm::mat3>, "ez::is_mat_v is incorrect");
static_assert(ez::is_mat_v<glm::mat4>, "ez::is_mat_v is incorrect");
static_assert(ez::is_mat_v<glm::mat2x3>, "ez::is_mat_v is incorrect");
static_assert(ez::is_mat_v<glm::mat2x4>, "ez::is_mat_v is incorrect");
static_assert(ez::is_mat_v<glm::mat3x4>, "ez::is_mat_v is incorrect");

static_assert(!ez::is_mat_strict_v<glm::vec2>, "ez::is_mat_strict_v is incorrect");
static_assert(ez::is_mat_strict_v<glm::mat2>, "ez::is_mat_strict_v is incorrect");
static_assert(!ez::is_mat_strict_v<float>, "ez::is_mat_strict_v is incorrect");

static_assert(std::is_same_v<float, ez::vec_value_t<glm::vec1>>, "ez::vec_value_t is incorrect");
static_assert(std::is_same_v<float, ez::vec_value_t<glm::vec2>>, "ez::vec_value_t is incorrect");
static_assert(std::is_same_v<float, ez::vec_value_t<glm::vec3>>, "ez::vec_value_t is incorrect");
static_assert(std::is_same_v<float, ez::vec_value_t<glm::vec4>>, "ez::vec_value_t is incorrect");

static_assert(ez::is_iterator_v<glm::vec2*>, "ez::is_iterator_v is incorrect");
static_assert(ez::is_iterator_v<glm::vec2* const> == false, "ez::is_iterator_v is incorrect");
static_assert(ez::is_iterator_v<const glm::vec2*> == true, "ez::is_iterator_v is incorrect");
static_assert(ez::is_output_iterator_v<const glm::vec2*> == false, "ez::is_output_iterator_v is incorrect");

void print(int a) {
	fmt::print("Expanded to 1!\n");
	fmt::print("{}\n", a);
}
void print(int a, int b) {
	fmt::print("Expanded to 2!\n");
	fmt::print("{}, {}\n", a, b);
}
void print(int a, int b, int c) {
	fmt::print("Expanded to 3!\n");
	fmt::print("{}, {}, {}\n", a, b, c);
}

// We can expand an interator into a set of arguments at compile time, but only if we wrap the call to the function
template<typename Iter>
struct CallExpanded {
	Iter iter;

	template<std::size_t N, typename... Args>
	decltype(auto) call(Args... args) {
		if constexpr (N == 0) {
			return print(args...);
		}
		else {
			return call<N-1>(args..., *iter++);
		}
	}
};

int main(int argc, char ** argv) {
	fmt::print("Meta test is mostly compile time.\n");

	std::array<int, 3> val{{ 0, 1, 2 }};
	using Iter = std::array<int, 3>::iterator;

	CallExpanded<Iter>{val.begin()}.call<3>();

	glm::vec2 p0{};
	glm::mat3 m0{};
	float f0;
	
	assert(ez::value_ptr(p0) == &p0[0]);
	assert(ez::value_ptr(m0) == &m0[0][0]);
	assert(ez::value_ptr(f0) == &f0);

	return 0;
}