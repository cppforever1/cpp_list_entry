#pragma once

#include <Windows.h>
#include <cstddef>
#include <iterator>
#include <type_traits>

template <typename T, LIST_ENTRY T::*Member>
class cpp_entry_list
{
private:
    static T *from_entry(PLIST_ENTRY entry)
    {
        auto member_ptr = &(reinterpret_cast<T *>(0)->*Member);
        auto offset = reinterpret_cast<std::ptrdiff_t>(member_ptr);

        return reinterpret_cast<T *>(
            reinterpret_cast<char *>(entry) - offset);
    }

    static LIST_ENTRY *to_entry(T *object)
    {
        return &(object->*Member);
    }

public:
    class iterator
    {
        friend class cpp_entry_list;

        PLIST_ENTRY current_{};
        PLIST_ENTRY head_{};

        iterator(PLIST_ENTRY current, PLIST_ENTRY head)
            : current_(current),
              head_(head)
        {
        }

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        iterator() = default;

        reference operator*() const
        {
            return *from_entry(current_);
        }

        pointer operator->() const
        {
            return from_entry(current_);
        }

        iterator &operator++()
        {
            current_ = current_->Flink;
            return *this;
        }

        iterator operator++(int)
        {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator &operator--()
        {
            current_ = current_->Blink;
            return *this;
        }

        bool operator==(const iterator &rhs) const
        {
            return current_ == rhs.current_;
        }

        bool operator!=(const iterator &rhs) const
        {
            return !(*this == rhs);
        }
    };

    cpp_entry_list()
    {
        initialize_link(&head_);
    }

    bool empty() const
    {
        return head_.Flink == &head_;
    }

    iterator begin()
    {
        return iterator(head_.Flink, &head_);
    }

    iterator end()
    {
        return iterator(&head_, &head_);
    }

    static bool is_linked(const T *object)
    {
        auto const &entry = object->*Member;

        return entry.Flink != &entry &&
               entry.Blink != &entry;
    }

    static void initialise_node(T *object)
    {
        initialize_link(&(object->*Member));
    }

    void push_front(T *object)
    {
        insert_head(
            &head_,
            to_entry(object));
    }

    void push_back(T *object)
    {
        insert_tail(
            &head_,
            to_entry(object));
    }

    T *pop_front()
    {
        if (empty())
            return nullptr;

        auto entry = remove_head(&head_);

        initialize_link(entry);

        return from_entry(entry);
    }

    T *pop_back()
    {
        if (empty())
            return nullptr;

        auto entry = remove_tail(&head_);

        initialize_link(entry);

        return from_entry(entry);
    }

    bool erase(T *object)
    {
        auto entry = to_entry(object);

        if (!is_linked(object))
            return false;

        remove_entry(entry);

        initialize_link(entry);

        return true;
    }

    iterator erase(iterator pos)
    {
        if (pos == end())
            return end();

        auto current = pos.current_;
        auto next = current->Flink;

        remove_entry(current);

        initialize_link(current);

        return iterator(next, &head_);
    }

    template <typename Predicate>
    size_t remove_if(Predicate pred)
    {
        size_t removed = 0;

        for (auto it = begin(); it != end();)
        {
            if (pred(*it))
            {
                it = erase(it);
                ++removed;
            }
            else
            {
                ++it;
            }
        }

        return removed;
    }

    void clear()
    {
        while (!empty())
        {
            auto entry = remove_head(&head_);
            initialize_link(entry);
        }
    }

private:
    static void initialize_link(PLIST_ENTRY entry)
    {
        entry->Flink = entry;
        entry->Blink = entry;
    }

    static void insert_head(PLIST_ENTRY head, PLIST_ENTRY entry)
    {
        auto first = head->Flink;
        entry->Flink = first;
        entry->Blink = head;
        first->Blink = entry;
        head->Flink = entry;
    }

    static void insert_tail(PLIST_ENTRY head, PLIST_ENTRY entry)
    {
        auto last = head->Blink;
        entry->Flink = head;
        entry->Blink = last;
        last->Flink = entry;
        head->Blink = entry;
    }

    static void remove_entry(PLIST_ENTRY entry)
    {
        entry->Blink->Flink = entry->Flink;
        entry->Flink->Blink = entry->Blink;
    }

    static PLIST_ENTRY remove_head(PLIST_ENTRY head)
    {
        auto entry = head->Flink;
        remove_entry(entry);
        return entry;
    }

    static PLIST_ENTRY remove_tail(PLIST_ENTRY head)
    {
        auto entry = head->Blink;
        remove_entry(entry);
        return entry;
    }

    LIST_ENTRY head_{};
};