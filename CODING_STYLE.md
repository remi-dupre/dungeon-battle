Files
=======

Extension .cpp, .hpp

Indentation is four (4) spaces, remove all trailing spaces. There is a blank line at the end of every file.

Do not use `using namespace` (except in certain cases, for instance `std::literals::string_litterals`)

Includes
========

Blank line between the different libraries
```cpp
#include <algorithm> // libraries (STL then others)
#include <cmath>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "game.hpp" // Project code
```
Two blank lines after includes (and other preprocessing macros)

Function and variable naming
============================

| Type                                       | naming                    |
|--------------------------------------------|---------------------------|
| File                                       | snake_case (.hpp / .cpp)  |
| local, static and global variable          | snake_case                |
| Member variable                            | snake_case                |
| Function, static method                    | snake_case                |
| Member function                            | camlCase                  |
| Type (struct, class, union, enum, typedef) | PascalCase                |
| Template parameter                         | PascalCase                |
| Namespace                                  | snake_case                |
| Macro (avoid them)                         | UPPER_CASE                |

Variables
=========

Built-in types:
```cpp
int n;
int& m = n;
int* m = &n;
```
Objects
```cpp
std::tuple<int, int> vec(3, 2); // (std::make_tuple would be better here)
```
Use `auto` wisely, when the type is very long or clearly defined.

Functions
=========
```cpp
int f()
{
    return 0;
}

template <typename T>
int* my_function(const T& t, int* p)
{
    int x = 3; // Prefer declarations at the beginning of the scope
    // Blank line after declarations

    function();
    other_function(3);
    return nullptr; // Always use nullptr
}
```

Control flow
============

Blank line before `if`, `for`, `while`, `switch`
One space before the first parenthesis of the condition
```cpp
if (condition)
{
    //...
}
else if (condition)
{
    //...
}
else
{
    //...
}

for (int n = 0; n < 10; n++)
{
    //...
}

for (very_long_declaration;
     very_long_condition;
     very_long_step)
{
    //...
}

while (condition)
{
     //...
}

switch (n)
{
case something:
    //...
    break; // Don't forget to break
case something_else:
    //...
    [[fallthrough]] // Use the attribute for volontary fallthroughs (even if not supported by g++)
default:
    //...
}
```

Classes definition
==================

```cpp
struct MyStruct
{
    float x; // Avoid declaration with no access modifier before
    //except in structs where everything is public
private:
    int n;

public:
    MyStruct(int n) : n(n) {}
    MyStruct() : MyStruct(0) {}

    bool operator==(MyStruct s)
    {
        // ...
    }
};

// Prefer strongly typed enums, unless you have a good reason to use classic ones
enum class Direction
{
    Up,
    Left,
    Down,
    Right
};

namespace my_space
{

}
```

Macros & Constants
==================

Avoid macro definition, use
```cpp
constexpr int HAUTEUR = 800;
```
instead of
```cpp
#define HAUTEUR 800
```


For functions, use templates instead of macros, use
```cpp
template <typename T>
inline constexpr T max (T a, T b)
{
   return a > b ? a : b;
}
```
not
```cpp
#define MAX(a, b) ((a) > (b)) ? (a) : (b)
```

Miscellaneous
=============

ALWAYS use `nullptr`, NEVER `NULL` or `0` for null pointers.

Casts: use c++'s casts
```cpp
float x = static_cast<float>(n);
```

Tests
=====

We use cxxtest for tests, tests are in `tests/test_testname.hpp` files

Documentation
=============

We use doxygen to generate documentation

Use this style of documentation:
```cpp
/**
 * \brief Short description
 * \param p What is argument p
 * \return Return value
 *
 * Complete documentation
 */
class Class
{
    // ...

    int var; ///< Variable description
};

/**
 * \brief Enum description
 *
 * Complete documentation
 */
enum class Enum
{
    One, ///< Description of One
    Two, ///< Description of Two
};
```
