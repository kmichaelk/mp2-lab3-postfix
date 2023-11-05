#include <gtest.h>
#include "stack.h"

TEST(TStack, can_create_stack_with_positive_length)
{
    ASSERT_NO_THROW(TStack<int> st(5));
}

TEST(TStack, cant_create_stack_with_too_large_length)
{
    EXPECT_ANY_THROW(TStack<int> st(MAX_STACK_SIZE + 1));
}

TEST(TStack, fresh_stack_is_empty)
{
    TStack<int> st(1);
    EXPECT_EQ(true, st.empty());
}

TEST(TStack, empty_stack_is_empty)
{
    TStack<int> st(2);
    st.push(1);
    st.pop();
    EXPECT_EQ(true, st.empty());
}

TEST(TStack, pop_removes_element)
{
    const int items = 5;
    TStack<int> st(items);
    for (int i = 0; i < items; i++)
    {
        st.push(i);
    }
    st.pop();
    EXPECT_EQ(items - 1, st.size());
}

TEST(TStack, dynamic_memory_allocation_works)
{
    const int items = 6;
    TStack<int> st(items / 2);
    for (int i = 0; i < items; i++)
    {
        st.push(i);
    }
    EXPECT_EQ(items, st.size());
}

TEST(TStack, top_retrieves_last_element)
{
    TStack<int> st(5);
    st.push(1);
    st.push(2);
    st.push(3);
    EXPECT_EQ(3, st.top());
}
