# cpp_list_entry

`cpp_list_entry` is a header-only C++ wrapper for Windows `LIST_ENTRY` intrusive lists. A `LIST_ENTRY` is a doubly linked node containing forward and backward links. Embed one in each object, then use `cpp_entry_list` to link and traverse those objects.

The list does not allocate or own objects; each object must remain alive while linked. An embedded node can belong to only one list at a time.

## Requirements

- Windows and a C++20-compatible compiler for this CMake project (the header API requires C++11 or newer)
- CMake 3.10 or newer

## Use

Include `cpp_list_entry.hpp` and provide the member pointer for the embedded link:

```cpp
#include "cpp_list_entry.hpp"

struct item
{
    int value;
    LIST_ENTRY link;
};

using item_list = cpp_entry_list<item, &item::link>;

int main()
{
    item first{1};
    item second{2};
    item_list::initialise_node(&first);
    item_list::initialise_node(&second);

    item_list items;
    items.push_back(&first);
    items.push_back(&second);

    for (item& entry : items)
        entry.value += 1;

    item* removed = items.pop_front();
    return removed == &first ? 0 : 1;
}
```

Each object must have its list node initialized before it is linked. A node may be linked into only one list at a time. Removing or clearing a node reinitializes its link.
Each object must have its list node initialized before it is linked. Removing an object does not destroy it, and removing or clearing a node reinitializes its link.

The list supports `empty`, `begin`/`end`, `push_front`/`push_back`, `pop_front`/`pop_back`, `erase`, `remove_if`, and `clear`. `is_linked` and `initialise_node` are static helpers on the list type.

## Build and test

From a Visual Studio developer environment, configure, build, and run the tests with:

```powershell
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```