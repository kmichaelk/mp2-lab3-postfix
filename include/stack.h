#ifndef __STACK_H__
#define __STACK_H__

#include "list.h"
#include <stdexcept>

#define _STACK_STR(x) #x
#define _STACK_EXPAND(x) _STACK_STR(x)

const size_t MAX_STACK_SIZE = 1000;

template<typename T>
class TStack
{
private:
    TDynamicList<T> list;

    void require_not_empty() const
    {
        if (empty())
            throw std::logic_error("Stack is empty");
    }

    void ensure_space_sufficient() const
    {
        if (size() == MAX_STACK_SIZE)
            throw std::length_error("Stack overflow");
    }
public:
    explicit
    TStack(size_t initial_capacity = 8)
        : list(initial_capacity <= MAX_STACK_SIZE
            ? initial_capacity
            : throw std::out_of_range("Stack size cannot exceed " _STACK_EXPAND(MAX_STACK_SIZE)))
    {}

    void push(const T& element)
    {
        ensure_space_sufficient();
        list.push_back(element);
    }
    void push(T&& element)
    {
        ensure_space_sufficient();
        list.push_back(element);
    }

    T& top()
    {
        require_not_empty();
        return list.tail();
    }

    void pop()
    {
        require_not_empty();
        list.remove(list.size() - 1);
    }

    [[nodiscard]]
    T pop_element()
    {
        require_not_empty();

        T element = list[list.size() - 1];
        list.remove(list.size() - 1);
        return element;
    }

    bool empty() const noexcept(noexcept(list.empty()))
    {
        return list.empty();
    }

    size_t size() const noexcept(noexcept(list.size()))
    {
        return list.size();
    }

    bool operator==(const TStack& other) const noexcept
    {
        return list == other.list;
    }
};

#undef _STACK_EXPAND
#undef _STACK_STR

#endif // __STACK_H__
