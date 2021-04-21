# ez-meta
Meta programming templates to check certain common type requirements.

### CMake
```cmake
find_package(ez-meta CONFIG REQUIRED)
target_link_libraries(my_target PRIVATE ez::meta)
```

### C++
```cpp
// Mostly just a single header
#include <ez/meta.hpp>

#include <vector>

using container_t = std::vector<int>;
using iterator_t = container_t::iterator;
using adaptor_t = std::back_insert_iterator<container_t>;

using pointer_t = int*;

static_assert(ez::is_iterator_v<iterator_t>, 
	"Checking if something is an iterator");

static_assert(ez::is_iterator_writable_v<adaptor_t, int>, 
	"Checking if we can assign int values");

static_assert(ez::is_random_iterator_v<iterator_t>, 
	"Checking category of iterator");
	
static_assert(ez::is_bidirectional_iterator_v<iterator_t>, 
	"Checking category of iterator");
	
static_assert(ez::is_forward_iterator_v<iterator_t>, 
	"Checking category of iterator");

static_assert(ez::is_output_iterator_v<adaptor_t>, 
	"Checking category of iterator");

static_assert(ez::is_random_iterator_v<pointer_t>,
	"Pointers count as random access iterators");

static_assert(ez::is_iterator_writable_v<pointer_t, int>,
	"Checking if we can assign to dereferenced iterator");
```
