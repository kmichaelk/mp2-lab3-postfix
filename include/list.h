#ifndef __LIST_H__
#define __LIST_H__

#include <stdexcept>

template<class T>
class TDynamicList {
private:
    T *pMem;
    size_t capacity;
    size_t length;
public:
    explicit
    TDynamicList(size_t initial_capacity = 8)
            : capacity(initial_capacity > 0
                ? initial_capacity
                : throw std::invalid_argument("Initial list capacity should be greater than 0"))
            , length(0)
            , pMem(new T[initial_capacity])
    {}
    TDynamicList(const TDynamicList& src)
            : capacity(src.capacity)
            , length(src.length)
            , pMem(new T[src.capacity])
    {
        std::copy(src.pMem, src.pMem + length, pMem);
    }
    TDynamicList(TDynamicList&& src) noexcept
            : pMem(nullptr)
    {
        swap(*this, src);
    }

    ~TDynamicList()
    {
        delete[] pMem;
    }

    T* begin() const
    {
        return pMem;
    }
    T* end() const
    {
        return pMem + length;
    }

    [[nodiscard]]
    size_t size() const noexcept
    {
        return length;
    }

    [[nodiscard]]
    bool empty() const noexcept
    {
        return length == 0;
    }

    void push_back(const T& element)
    {
        if (length == capacity)
        {
            //
            const size_t new_capacity = capacity * 2;
            //

            capacity = new_capacity;
            TDynamicList<T> tmp(*this);
            swap(*this, tmp);
        }
        pMem[length++] = element;
    }
    void push_back(T&& element)
    {
        push_back(element);
    }

    void remove(size_t idx)
    {
        std::copy(pMem + idx + 1, pMem + length, pMem + idx);
        length--;
    }

    T& tail()
    {
        return pMem[length - 1];
    }

    T& operator[](const size_t idx)
    {
        return const_cast<T&>(std::as_const(*this)[idx]);
    }
    const T& operator[](const size_t idx) const
    {
        return pMem[idx];
    }

    bool operator==(const TDynamicList& other) const noexcept
    {
        if (length != other.length)
            return false;

        for (size_t i = 0; i < length; i++)
            if (pMem[i] != other.pMem[i])
                return false;

        return true;
    }
    bool operator!=(const TDynamicList& other) const noexcept
    {
        return !(*this == other);
    }

    TDynamicList& operator=(const TDynamicList& other)
    {
        if (this == &other)
            return *this;

        TDynamicList<T> tmp(other);
        swap(*this, tmp);
        return *this;
    }
    TDynamicList& operator=(TDynamicList&& other) noexcept
    {
        swap(*this, other);
        return *this;
    }

    void clear()
    {
        length = 0;
    }

    void shrink_to_fit()
    {
        capacity = length;
        TDynamicList<T> tmp(*this);
        swap(*this, tmp);
    }

    friend void swap(TDynamicList& lhs, TDynamicList& rhs) noexcept
    {
        std::swap(lhs.pMem, rhs.pMem);
        std::swap(lhs.capacity, rhs.capacity);
        std::swap(lhs.length, rhs.length);
    }
};

#endif // __LIST_H__
