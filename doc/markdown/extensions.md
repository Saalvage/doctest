## Extensions

The doctest header doesn't include any external or stdlib headers in it's interface part in order to provide the most optimal build times but that means it is limited in what it can provide as functionality => that's when extensions come into play. They are located as header files in [`doctest/extensions`](../../doctest/extensions) and each of them is documented in a section here.

# [Utils](../../doctest/extensions/doctest_util.h)

nothing here yet...

# [Distributed tests with MPI](../../doctest/extensions/doctest_mpi.h)

[Bruno Maugars and Bérenger Berthoul, ONERA]

Testing code over distributed processes requires support from the testing framework. **Doctest** support for MPI parallel communication is provided in the ```"doctest/extensions/doctest_mpi.h"``` header.

## Example

See [**the complete test**](../../examples/mpi/mpi.cpp) and [**the configuration of main()**](../../examples/mpi/main.cpp)

### MPI_TEST_CASE

```c++
#include "doctest/extensions/doctest_mpi.h"

int my_function_to_test(MPI_Comm comm) {
  int rank;
  MPI_Comm_rank(comm,&rank);
  if (rank == 0) {
    return 10;
  }
  return 11;
}


MPI_TEST_CASE("test over two processes",2) { // Parallel test on 2 processes
  int x = my_function_to_test(test_comm);
  
  MPI_CHECK( 0,  x==10 ); // CHECK for rank 0, that x==10
  MPI_CHECK( 1,  x==11 ); // CHECK for rank 1, that x==11
}
```

An ```MPI_TEST_CASE``` is like a regular ```TEST_CASE```, except it takes a second argument, which is the number of processes needed to run the test.  If the number of processes is less than 2, the test will fail. If the number of processes is greater than or equal to 2, it will create a sub-communicator over 2 processes, called ```test_comm```, and execute the test over these processes. Three objects are provided by ```MPI_TEST_CASE```: 
 * ```test_comm```, of type ```MPI_Comm```: the mpi communicator on which the test is running,
 * ```test_rank``` and ```test_nb_procs```, two ```int``` giving respectively the rank of the current process and the size of the communicator for ```test_comm```. These last two are just here for convenience and could be retrieved from ```test_comm```.
 
We always have:

```c++
MPI_TEST_CASE("my_test",N) {
  CHECK( test_nb_procs == N );
  MPI_CHECK( i, test_rank==i ); // for any i<N
}
```

### Assertions
It is possible to use regular assertions in an ```MPI_TEST_CASE```. MPI-specific assertions are also provided and are all prefixed with ```MPI_``` (```MPI_CHECK```, ```MPI_ASSERT```...). The first argument is the rank for which they are checked, and the second is the usual expression to check.

## The main entry points and mpi reporters

You need to launch the unit tests with an ```mpirun``` command:
```
mpirun -np 2 unit_test_executable.exe
```

```MPI_Init``` should be called before running the unit tests. Also, using the default console reporter will result in each process writing everything in the same place, which is not what we want. Two reporters are provided and can be enabled. A complete ```main()``` would be:


```c++
#define DOCTEST_CONFIG_IMPLEMENT

#include "doctest/extensions/doctest_mpi.h"

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  doctest::Context ctx;
  ctx.setOption("reporters", "MpiConsoleReporter");
  ctx.setOption("reporters", "MpiFileReporter");
  ctx.setOption("force-colors", true);
  ctx.applyCommandLine(argc, argv);

  int test_result = ctx.run();

  MPI_Finalize();

  return test_result;
}
```

### MpiConsoleReporter

The ```MpiConsoleReporter``` should be substituted to the default reporter. It does the same as the default console reporter for regular assertions, but only outputs on process 0. For MPI test cases, if there is a failure it tells the process that failed

```
[doctest] doctest version is "2.4.0"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:    171 |    171 passed |      0 failed |      0 skipped
[doctest] assertions:    864 |    864 passed |      0 failed |
[doctest] Status: SUCCESS!
std_e_mpi_unit_tests
[doctest] doctest version is "2.4.0"
[doctest] run with "--help" for options
===============================================================================
path/to/test.cpp:30:
TEST CASE: my test case 

On rank [2] : path/to/test.cpp:35: CHECK( x==-1 ) is NOT correct!
  values: CHECK( 0 == -1 )

===============================================================================
[doctest] test cases:      2 |      2 passed |      0 failed |      0 skipped
[doctest] assertions:      2 |      2 passed |      0 failed |
[doctest] Status: SUCCESS!
===============================================================================
[doctest] glob assertions:      5 |      4 passed |      1 failed |
===============================================================================
[doctest] fail on rank:     
    -> On rank [2] with 1 test failed
[doctest] Status: FAILURE!
```

### MpiFileReporter
The ```MpiFileReporter``` will just print the result of each process in its own file, named ```doctest_[rank].log```. Only use this reporter as a debug facility if you want to know what is going on exactly when a parallel test case is failing.

### Other reporters
Other reporters (jUnit, XML) are not supported directly, which mean that you can always print the result of each process to its own file, but there is (currently) no equivalent of the ```MpiConsoleReporter``` that will aggregate the results of all processes.


## Note

This feature is provided to unit-test mpi-distributed code. It is **not** a way to parallelize many unit tests over several processes (for that, see [**the example python script**](../../examples/range_based_execution.py)).

## TODO

 * Pass ```s``` member variable of ```ConsoleReporter``` as an argument to member functions so we can use them with another object (would help to factorize ```MPIConsoleReporter```)
 * Only MPI_CHECK tested. MPI_REQUIRE, exception handling: nothing tested
 * If the number of processes is not enough, prints the correct message, but then deadlocks (comes from ```MPI_Probe``` in ```MpiConsoleReporter```)
 * [[maybe_unused]] is C++17
 * More testing, automatic testing
 * Packaging: create a new target ```mpi_doctest```? (probably cleaner to depend explicitly on MPI for mpi/doctest.h)
 * Later, maybe: have a general mechanism to represent assertions so we can separate the report format (console, xml, junit...) from the reporting strategy (sequential vs. MPI)



# [Stringification of the STL (and beyond)](../../doctest/extensions/stl_stringifier.h)

`<doctest/extensions/stl_stringify.h>` provides stringification for all components of the standard C++ library.

The C++ standard version is automatically checked and types belonging to a newer, unsupported standard can never (accidentally) be enabled.

## Example

```cpp
#include <doctest/extensions/stl_stringify.h>

TEST_CASE("Test prime generator") {
    std::vector<int> primes = generatePrimes(5);
    CHECK(primes == std::vector{ 2, 3, 5, 7, 11 });
}
```
↓
```
path/to/test.cpp(5): ERROR: CHECK( primes == std::vector{ 2, 3, 5, 7, 11 } ) is NOT correct!
  values: CHECK( [1, 2, 3, 5, 7] == [2, 3, 5, 7, 11] )
```

## Configuration

### **DOCTEST_STL_STRINGIFY_NO_COMMON_INCLUDES**
By default, a few very common standard classes are selected (among them are `vector`, `list`, `set` and `unordered_map`).
You can disable this standard selection by defining `DOCTEST_STL_STRINGIFY_NO_COMMON_INCLUDES` before including the header.

### Header flags
For every standard header there exists a flag with which classes contained within it can be enabled/disable (depending on `DOCTEST_STL_STRINGIFY_FLIP`).

| Class | Flag | Default | Header | Example
| - | - | - | - | - |
| [array](https://en.cppreference.com/w/cpp/container/array) | DOCTEST_STL_STRINGIFY_FLAG_ARRAY | ❌ | `<array>` | `[1, 2, 3, 4]` |
| [span](https://en.cppreference.com/w/cpp/container/span) | DOCTEST_STL_STRINGIFY_FLAG_SPAN | ❌ | `<span>` | `[1, 2, 3, 4]` |
| [valarray](https://en.cppreference.com/w/cpp/numeric/valarray) | DOCTEST_STL_STRINGIFY_FLAG_VALARRAY | ❌ | `<valarray>` | `[1, 2, 3, 4]` |
| [initializer_list](https://en.cppreference.com/w/cpp/utility/initializer_list) | DOCTEST_STL_STRINGIFY_FLAG_INITIALIZER_LIST | ✔️ | `<initializer_list>` | `[1, 2, 3, 4]` |
| [vector](https://en.cppreference.com/w/cpp/container/vector) | DOCTEST_STL_STRINGIFY_FLAG_VECTOR | ✔️ | `<vector>` | `[1, 2, 3, 4]` |
| [deque](https://en.cppreference.com/w/cpp/container/deque) | DOCTEST_STL_STRINGIFY_FLAG_DEQUE | ❌ | `<deque>` | `[1, 2, 3, 4]` |
| [list](https://en.cppreference.com/w/cpp/container/list) | DOCTEST_STL_STRINGIFY_FLAG_LIST | ✔️ | `<list>` | `[1, 2, 3, 4]` |
| [forward_list](https://en.cppreference.com/w/cpp/container/forward_list) | DOCTEST_STL_STRINGIFY_FLAG_FORWARD_LIST | ❌ | `<forward_list>` | `[1, 2, 3, 4]` |
| [stack](https://en.cppreference.com/w/cpp/container/stack) | DOCTEST_STL_STRINGIFY_FLAG_STACK | ❌ | `<stack>` | `[1, 2, 3, 4]` |
| [queue](https://en.cppreference.com/w/cpp/container/queue) | DOCTEST_STL_STRINGIFY_FLAG_QUEUE | ❌ | `<queue>` | `[1, 2, 3, 4]` |
| [priority_queue](https://en.cppreference.com/w/cpp/container/priority_queue) | DOCTEST_STL_STRINGIFY_FLAG_QUEUE | ❌ | `<queue>` | `[1, 2, 3, 4]` |
| [set](https://en.cppreference.com/w/cpp/container/set) | DOCTEST_STL_STRINGIFY_SET | ✔️ | `<set>` | `[1, 2, 3, 4]` |
| [multiset](https://en.cppreference.com/w/cpp/container/multiset) | DOCTEST_STL_STRINGIFY_SET | ✔️ | `<set>` | `[1, 2, 3, 4]` |
| [unordered_set](https://en.cppreference.com/w/cpp/container/unordered_set) | DOCTEST_STL_STRINGIFY_UNORDERED_SET | ❌ | `<unordered_set>` | `{1, 2, 3, 4}` |
| [unordered_multiset](https://en.cppreference.com/w/cpp/container/unordered_multiset) | DOCTEST_STL_STRINGIFY_UNORDERED_SET | ❌ | `<unordered_set>` | `{1, 2, 3, 4}` |
| [map](https://en.cppreference.com/w/cpp/container/map) | DOCTEST_STL_STRINGIFY_MAP | ❌ | `<map>` | `{(1, "one"), (2, "two"), (3, "three"), (4, "four")}` |
| [multimap](https://en.cppreference.com/w/cpp/container/multimap) | DOCTEST_STL_STRINGIFY_MAP | ❌ | `<map>` | `{(1, "one"), (2, "two"), (3, "three"), (4, "four")}` |
| [unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map) | DOCTEST_STL_STRINGIFY_UNORDERED_MAP | ✔️ | `<unordered_map>` | `{(1, "one"), (2, "two"), (3, "three"), (4, "four")}` |
| [unordered_multimap](https://en.cppreference.com/w/cpp/container/unordered_multimap) | DOCTEST_STL_STRINGIFY_UNORDERED_MAP | ✔️ | `<unordered_map>` | `{(1, "one"), (2, "two"), (3, "three"), (4, "four")}` |
| [integer_sequence](https://en.cppreference.com/w/cpp/utility/integer_sequence) | DOCTEST_STL_STRINGIFY_UTILITY | ❌ | `<utility>` | `[1, 2, 3, 4]` |
| [pair](https://en.cppreference.com/w/cpp/utility/pair) | DOCTEST_STL_STRINGIFY_UTILITY | ❌ | `<utility>` | `(1, "one")` |
| [tuple](https://en.cppreference.com/w/cpp/utility/tuple) | DOCTEST_STL_STRINGIFY_TUPLE | ❌ | `<tuple>` | `(true, false, true, 1, "one")` |
| [ratio](https://en.cppreference.com/w/cpp/numeric/ratio) | DOCTEST_STL_STRINGIFY_RATIO | ❌ | `<ratio>` | `1/100` | |
| [variant](https://en.cppreference.com/w/cpp/utility/variant) | DOCTEST_STL_STRINGIFY_VARIANT | ❌ | `<variant>` | `1` (currently held value, stringified) |
| [monostate](https://en.cppreference.com/w/cpp/utility/variant/monostate) | DOCTEST_STL_STRINGIFY_VARIANT | ❌ | `<variant>` | `monostate`
| [optional](https://en.cppreference.com/w/cpp/utility/optional) | DOCTEST_STL_STRINGIFY_OPTIONAL | ❌ | `<optional>` | `1` (held value or nullopt, stringified) |
| [nullopt_t](https://en.cppreference.com/w/cpp/utility/optional/nullopt_t) | DOCTEST_STL_STRINGIFY_OPTIONAL | ❌ | `<optional>` | `nullopt` |
| [type_info](https://en.cppreference.com/w/cpp/types/type_info) | DOCTEST_STL_STRINGIFY_TYPE_INFO | ❌ | `<typeinfo>` | (implementation defined) |
| [chrono::time_point](https://en.cppreference.com/w/cpp/chrono/time_point) | DOCTEST_STL_STRINGIFY_CHRONO | ❌ | `<chrono>` | `1970-01-01 00:00:00.000` |

### **DOCTEST_STL_STRINGIFY_FLIP**
By default, the above flags enable stringification of classes. By defining `DOCTEST_STL_STRINGIFY_FLIP` this behavior can be inverted. All classes have stringification enabled at first and it can be disabled by defining their flag.

Only defining `DOCTEST_STL_STRINGIFY_FLIP` is a simple way to enable stringification of all standard classes.

(Note: Not defining `DOCTEST_STL_STRINGIFY_NO_COMMON_INCLUDES` will enable classes, even if they are explicitly disabled. But it will never disable a class.)

### Across translation units
The header should be included in all source files containing test code and the flags should not vary among translation units. The results of not adhering to this are undefined.

---------------

[Home](readme.md#reference)

<p align="center"><img src="../../scripts/data/logo/icon_2.svg"></p>
