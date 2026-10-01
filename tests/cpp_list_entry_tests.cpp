#include <cassert>
#include <cstddef>
#include <vector>

#include "cpp_list_entry.hpp"

struct item
{
    int value;
    LIST_ENTRY link;
};

using item_list = cpp_entry_list<item, &item::link>;

// Test suite for cpp_entry_list functionality
static void test_empty_and_push_pop()
{
    // Test pushing and popping items from the list

    // Initialize the items before adding them to the list
    item first{1};
    item second{2};
    item third{3};

    // Initialize the list nodes for each item
    item_list::initialise_node(&first);
    item_list::initialise_node(&second);
    item_list::initialise_node(&third);

    // Create the list and verify it is initially empty
    item_list items;

    // Verify the list is initially empty
    assert(items.empty());
    assert(items.pop_front() == nullptr);
    assert(items.pop_back() == nullptr);
    assert(!item_list::is_linked(&first));

    // Push items to the list and verify their linkage
    items.push_front(&second);
    items.push_front(&first);
    items.push_back(&third);

    // Verify the list is no longer empty and all items are linked
    assert(!items.empty());
    assert(item_list::is_linked(&first));
    assert(item_list::is_linked(&second));
    assert(item_list::is_linked(&third));

    assert(items.pop_front() == &first);
    assert(!item_list::is_linked(&first));
    assert(items.pop_back() == &third);
    assert(items.pop_front() == &second);
    assert(items.empty());
}

// Test suite for iteration and erase functionality
static void test_iteration_and_erase()
{
    // Test iterating over the list and erasing elements
    item first{1};
    item second{2};
    item third{3};

    // Initialize the list nodes for each item
    item_list::initialise_node(&first);
    item_list::initialise_node(&second);
    item_list::initialise_node(&third);

    // Create the list and add items to it
    item_list items;

    // Push items to the list in order: first, second, third
    items.push_back(&first);
    items.push_back(&second);
    items.push_back(&third);

    // Iterate over the list and collect the values for verification
    std::vector<int> values;
    for (const item &entry : items)
        values.push_back(entry.value);
    assert((values == std::vector<int>{1, 2, 3}));

    // Erase the second item and verify the list's integrity
    auto next = items.erase(++items.begin());
    assert(next->value == 3);
    assert(!item_list::is_linked(&second));
    assert(items.erase(&first));
    assert(!items.erase(&first));
    assert(items.begin()->value == 3);

    // Clear the list and verify it is empty
    items.clear();
    assert(items.empty());
    assert(!item_list::is_linked(&third));
}

// Test suite for remove_if functionality
static void test_remove_if()
{
    // Initialize the items before adding them to the list
    // Initialize the list nodes for each item
    item first{1};
    item second{2};
    item third{3};

    // Initialize the list nodes for each item
    item_list::initialise_node(&first);
    item_list::initialise_node(&second);
    item_list::initialise_node(&third);

    // Create the list and add items to it
    item_list items;
    items.push_back(&first);
    items.push_back(&second);
    items.push_back(&third);

    // Remove items from the list that satisfy the given condition
    const std::size_t removed = items.remove_if(
        [](const item &entry)
        { return entry.value % 2 != 0; });

    assert(removed == 2);
    assert(!item_list::is_linked(&first));
    assert(item_list::is_linked(&second));
    assert(!item_list::is_linked(&third));
    assert(items.begin()->value == 2);
    assert(++items.begin() == items.end());
}

int main()
{
    test_empty_and_push_pop();
    test_iteration_and_erase();
    test_remove_if();
}