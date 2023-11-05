#include <gtest.h>
#include "list.h"

TEST(TDynamicList, can_insert_to_begin)
{
    TDynamicList<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    list.insert(0, 5);

    EXPECT_EQ(5, list[0]);
}